/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.support.test.aupt;

import android.app.UiAutomation;
import android.os.Bundle;
import android.os.ParcelFileDescriptor;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

/**
 * GraphicsStatsMonitor is an internal monitor for AUPT to poll and track the information coming out
 * of the shell command, "dumpsys graphicsstats." In particular, the purpose of this is to see jank
 * statistics across the lengthy duration of an AUPT run.
 * <p>
 * To use the monitor, simply specify the options: trackJank true and jankInterval n, where n is
 * the polling interval in milliseconds. The default is 5 minutes. Also, it should be noted that
 * the trackJank option is unnecessary and this comment should be removed at the same time as it.
 * <p>
 * This graphics service monitors jank levels grouped by foreground process. Even when the process
 * is killed, the monitor will continue to track information, unless the buffer runs out of space.
 * This should only occur when too many foreground processes have been killed and the service
 * decides to clear itself. When pulling the information out of the monitor, these separate images
 * are combined to provide a single image as output. The linear information is preserved by simply
 * adding the values together. However, certain information such as the jank percentiles are
 * approximated using a weighted average.
 */
public class GraphicsStatsMonitor {
    private static final String TAG = "GraphicsStatsMonitor";

    public static final int MS_IN_SECS = 1000;
    public static final int SECS_IN_MIN = 60;
    public static final long DEFAULT_INTERVAL_RATE = 5 * SECS_IN_MIN * MS_IN_SECS;

    private Timer mIntervalTimer;
    private TimerTask mIntervalTask;
    private long mIntervalRate;
    private boolean mIsRunning;

    private Map<String, List<JankStat>> mGraphicsStatsRecords;


    public GraphicsStatsMonitor () {
        mIntervalTask = new TimerTask() {
            @Override
            public void run () {
                if (mIsRunning) {
                    grabStatsImage();
                }
            }
        };
        mIntervalRate = DEFAULT_INTERVAL_RATE;
        mIsRunning = false;
    }

    /**
     * Sets the monitoring interval rate if the monitor isn't currently running
     */
    public void setIntervalRate (long intervalRate) {
        if (mIsRunning) {
            Log.e(TAG, "Can't set interval rate for monitor that is already running");
        } else if (intervalRate > 0L) {
            mIntervalRate = intervalRate;
            Log.v(TAG, String.format("Set jank monitor interval rate to %d", intervalRate));
        }
    }

    /**
     * Starts to monitor graphics stats on the interval timer after clearing the stats
     */
    public void startMonitoring () {
        if (mGraphicsStatsRecords == null) {
            mGraphicsStatsRecords = new HashMap<>();
        }

        clearGraphicsStats();

        // Schedule a daemon timer to grab stats periodically
        mIntervalTimer = new Timer(true);
        mIntervalTimer.schedule(mIntervalTask, 0, mIntervalRate);

        mIsRunning = true;
        Log.d(TAG, "Started monitoring graphics stats");
    }

    /**
     * Stops monitoring graphics stats by canceling the interval timer
     */
    public void stopMonitoring () {
        mIntervalTimer.cancel();

        mIsRunning = false;
        Log.d(TAG, "Stopped monitoring graphics stats");
    }

    /**
     * Takes a snapshot of the graphics stats and incorporates them into the process stats history
     */
    public void grabStatsImage () {
        Log.v(TAG, "Grabbing image of graphics stats");
        List<JankStat> allStats = gatherGraphicsStats();

        for (JankStat procStats : allStats) {
            List<JankStat> history;
            if (mGraphicsStatsRecords.containsKey(procStats.packageName)) {
                history = mGraphicsStatsRecords.get(procStats.packageName);
                // Has the process been killed and restarted?
                if (procStats.isContinuedFrom(history.get(history.size() - 1))) {
                    // Process hasn't been killed and restarted; put the data
                    history.set(history.size() - 1, procStats);
                    Log.v(TAG, String.format("Process %s stats have not changed, overwriting data.",
                            procStats.packageName));
                } else {
                    // Process has been killed and restarted; append the data
                    history.add(procStats);
                    Log.v(TAG, String.format("Process %s stats were restarted, appending data.",
                            procStats.packageName));
                }
            } else {
                // Initialize the process stats history list
                history = new ArrayList<>();
                history.add(procStats);
                // Put the history list in the JankStats map
                mGraphicsStatsRecords.put(procStats.packageName, history);
                Log.v(TAG, String.format("New process, %s. Creating jank history.",
                        procStats.packageName));
            }
        }
    }

    /**
     * Aggregates the graphics stats for each process over its history. Merging specifications can
     * be found in the static method {@link JankStat#mergeStatHistory}.
     */
    public List<JankStat> aggregateStatsImages () {
        Log.d(TAG, "Aggregating graphics stats history");
        List<JankStat> mergedStatsList = new ArrayList<JankStat>();

        for (Map.Entry<String, List<JankStat>> record : mGraphicsStatsRecords.entrySet()) {
            String proc = record.getKey();
            List<JankStat> history = record.getValue();

            Log.v(TAG, String.format("Aggregating stats for %s (%d set%s)", proc, history.size(),
                    (history.size() > 1 ? "s" : "")));

            JankStat mergedStats = JankStat.mergeStatHistory(history);
            mergedStatsList.add(mergedStats);
        }

        return mergedStatsList;
    }

    /**
     * Clears all graphics stats history data for all processes
     */
    public void clearStatsImages () {
        mGraphicsStatsRecords.clear();
    }

    /**
     * Resets graphics stats for all currently tracked processes
     */
    public void clearGraphicsStats () {
        Log.d(TAG, "Reset all graphics stats");
        List<JankStat> existingStats = gatherGraphicsStats();
        for (JankStat stat : existingStats) {
            executeShellCommand(String.format("dumpsys gfxinfo %s reset", stat.packageName));
            Log.v(TAG, String.format("Cleared graphics stats for %s", stat.packageName));
        }
    }

    /*
     * Return JankStat objects from a stream representing the output of `dumpsys graphicsstats`
     *
     * This is broken out from gatherGraphicsStats for testing purposes
     */
    private List<JankStat> parseGraphicsStatsFromStream(BufferedReader stream) throws IOException {
        // TODO: this kind of stream filtering is much nicer using the Java 8 functional
        // primitives. Once AUPT goes to jdk8, we should refactor this.

        JankStat.StatPattern patterns[] = JankStat.StatPattern.values();
        List<JankStat> result = new ArrayList<>();
        JankStat nextStat = new JankStat(null, 0L, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        String line;

        // Split the stream by package
        while((line = stream.readLine()) != null) {
            if(JankStat.StatPattern.PACKAGE.parse(line) != null) {

                // When the output of `dumpsys graphicsstats` enters a new set of jank stats, we start
                // with a line matching the PACKAGE pattern; so we have to make a new JankStat for it and
                // save the old one.

                if(nextStat.packageName != null) {
                    Log.v(TAG, String.format("Gathered jank info from process %s.", nextStat.packageName));
                    result.add(nextStat);
                }

                nextStat = new JankStat(JankStat.StatPattern.PACKAGE.parse(line),
                        0L, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

            } else {

                // NOTE: we know these theoretically come in order, so we don't have to iterate
                // through the whole pattern array every time; but there is enough variation in
                // the code generating these log lines to justify a more input-robust parser

                for (JankStat.StatPattern p : patterns) {
                    if (p.getPattern() != null && p.parse(line) != null) {
                        switch (p) {
                            case STATS_SINCE:
                                nextStat.statsSince = Long.parseLong(p.parse(line));
                                break;
                            case TOTAL_FRAMES:
                                nextStat.totalFrames = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_JANKY:
                                nextStat.jankyFrames = Integer.valueOf(p.parse(line));
                                break;
                            case FRAME_TIME_50TH:
                                nextStat.frameTime50th = Integer.valueOf(p.parse(line));
                                break;
                            case FRAME_TIME_90TH:
                                nextStat.frameTime90th = Integer.valueOf(p.parse(line));
                                break;
                            case FRAME_TIME_95TH:
                                nextStat.frameTime95th = Integer.valueOf(p.parse(line));
                                break;
                            case FRAME_TIME_99TH:
                                nextStat.frameTime99th = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_MISSED_VSYNC:
                                nextStat.numMissedVsync = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_HIGH_INPUT_LATENCY:
                                nextStat.numHighLatency = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_SLOW_UI_THREAD:
                                nextStat.numSlowUiThread = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_SLOW_BITMAP_UPLOADS:
                                nextStat.numSlowBitmap = Integer.valueOf(p.parse(line));
                                break;
                            case NUM_SLOW_DRAW:
                                nextStat.numSlowDraw = Integer.valueOf(p.parse(line));
                                break;
                            default:
                                throw new RuntimeException(
                                        "Unexpected parsing state in GraphicsStateMonitor");
                        }
                    }
                }
            }
        }

        // Remember to add the last JankStat
        // We can't wrap this in the previous call because BufferedReader doesn't have a .peek()
        if(nextStat.packageName != null) {
            Log.v(TAG, String.format("Gathered jank info from process %s.", nextStat.packageName));
            result.add(nextStat);
        }

        return result;
    }

    /**
     * Return JankStat objects with metric data for all currently tracked processes
     */
    public List<JankStat> gatherGraphicsStats () {
        Log.v(TAG, "Gather all graphics stats");
        BufferedReader stream = executeShellCommand("dumpsys graphicsstats");

        try {
            return parseGraphicsStatsFromStream(stream);
        } catch (IOException exception) {
            Log.e(TAG, "Error with buffered reader", exception);
            return null;
        } finally {
            try {
                if (stream != null) {
                    stream.close();
                }
            } catch (IOException exception) {
                Log.e(TAG, "Error with closing the stream", exception);
            }
        }
    }

    /**
     * UiAutomation is included solely for the purpose of executing shell commands
     */
    private UiAutomation mUiAutomation;

    /**
     * Executes a shell command through UiAutomation and puts the results in an
     * InputStreamReader that is returned inside a BufferedReader.
     * @param command the command to be executed in the adb shell
     * @result a BufferedReader that reads the command output
     */
    public BufferedReader executeShellCommand (String command) {
        ParcelFileDescriptor stdout = getUiAutomation().executeShellCommand(command);

        BufferedReader stream = new BufferedReader(new InputStreamReader(
                new ParcelFileDescriptor.AutoCloseInputStream(stdout)));
        return stream;
    }

    /**
     * Sets the UiAutomation member for shell execution
     */
    public void setUiAutomation (UiAutomation uiAutomation) {
        mUiAutomation = uiAutomation;
    }

    /**
     * @return UiAutomation instance from Aupt instrumentation
     */
    public UiAutomation getUiAutomation () {
        return mUiAutomation;
    }
}
