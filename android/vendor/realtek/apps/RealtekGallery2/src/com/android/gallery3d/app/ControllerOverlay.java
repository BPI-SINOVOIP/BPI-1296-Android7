/*
 * Copyright (C) 2011 The Android Open Source Project
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

package com.android.gallery3d.app;

import android.view.View;

public interface ControllerOverlay {

  interface Listener {
    void onPlayPause(boolean bShowController,boolean bShowControlButtons);
    void onSeekStart();
    void onSeekMove(int time);
    void onSeekEnd(int time, int trimStartTime, int trimEndTime);
    void onShown();
    void onHidden();
    void onReplay();
    void onStop();
    void onInsReplay();
    void onCmSkip();
    void onFastForward();
    void onFastRewind();
    void onNext();
    void onPrevious();
    void onRepeatModeChange();
    void onShowOption();
  }

  void setListener(Listener listener);

  void setCanReplay(boolean canReplay);

  void setRepeatMode(int repeatMode, boolean bHasFocus);

  /**
   * @return The overlay view that should be added to the player.
   */
  View getView();

  void show(boolean bShowController,boolean bShowControlButtons);

  void showPlaying(boolean bShowController,boolean bShowControlButtons);

  void showPaused(boolean bShowController,boolean bShowControlButtons);

  void showEnded();

  void showLoading();

  void showErrorMessage(String message);

  void setTimes(int currentTime, int totalTime,
          int trimStartTime, int trimEndTime);
}
