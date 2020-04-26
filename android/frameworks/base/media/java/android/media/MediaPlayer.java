/*
 * Copyright (C) 2006 The Android Open Source Project
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

package android.media;

import android.annotation.IntDef;
import android.annotation.NonNull;
import android.annotation.Nullable;
import android.app.ActivityThread;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Parcel;
import android.os.Parcelable;
import android.os.Process;
import android.os.PowerManager;
import android.os.SystemProperties;
import android.provider.Settings;
import android.system.ErrnoException;
import android.system.OsConstants;
import android.util.Log;
import android.util.Pair;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.widget.VideoView;
import android.graphics.SurfaceTexture;
import android.media.AudioManager;
import android.media.MediaFormat;
import android.media.MediaTimeProvider;
import android.media.PlaybackParams;
import android.media.SubtitleController;
import android.media.SubtitleController.Anchor;
import android.media.SubtitleData;
// added by RTK
import android.net.TrafficStats;
import android.os.SystemProperties;
import android.media.NavigationRecord;
import android.media.MediaType;
// end of added by RTK
import android.media.SubtitleTrack.RenderingWidget;
import android.media.SyncParams;

import com.android.internal.util.Preconditions;

import libcore.io.IoBridge;
import libcore.io.Libcore;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.Runnable;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.Vector;
import java.lang.ref.WeakReference;

/**
 * MediaPlayer class can be used to control playback
 * of audio/video files and streams. An example on how to use the methods in
 * this class can be found in {@link android.widget.VideoView}.
 *
 * <p>Topics covered here are:
 * <ol>
 * <li><a href="#StateDiagram">State Diagram</a>
 * <li><a href="#Valid_and_Invalid_States">Valid and Invalid States</a>
 * <li><a href="#Permissions">Permissions</a>
 * <li><a href="#Callbacks">Register informational and error callbacks</a>
 * </ol>
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about how to use MediaPlayer, read the
 * <a href="{@docRoot}guide/topics/media/mediaplayer.html">Media Playback</a> developer guide.</p>
 * </div>
 *
 * <a name="StateDiagram"></a>
 * <h3>State Diagram</h3>
 *
 * <p>Playback control of audio/video files and streams is managed as a state
 * machine. The following diagram shows the life cycle and the states of a
 * MediaPlayer object driven by the supported playback control operations.
 * The ovals represent the states a MediaPlayer object may reside
 * in. The arcs represent the playback control operations that drive the object
 * state transition. There are two types of arcs. The arcs with a single arrow
 * head represent synchronous method calls, while those with
 * a double arrow head represent asynchronous method calls.</p>
 *
 * <p><img src="../../../images/mediaplayer_state_diagram.gif"
 *         alt="MediaPlayer State diagram"
 *         border="0" /></p>
 *
 * <p>From this state diagram, one can see that a MediaPlayer object has the
 *    following states:</p>
 * <ul>
 *     <li>When a MediaPlayer object is just created using <code>new</code> or
 *         after {@link #reset()} is called, it is in the <em>Idle</em> state; and after
 *         {@link #release()} is called, it is in the <em>End</em> state. Between these
 *         two states is the life cycle of the MediaPlayer object.
 *         <ul>
 *         <li>There is a subtle but important difference between a newly constructed
 *         MediaPlayer object and the MediaPlayer object after {@link #reset()}
 *         is called. It is a programming error to invoke methods such
 *         as {@link #getCurrentPosition()},
 *         {@link #getDuration()}, {@link #getVideoHeight()},
 *         {@link #getVideoWidth()}, {@link #setAudioStreamType(int)},
 *         {@link #setLooping(boolean)},
 *         {@link #setVolume(float, float)}, {@link #pause()}, {@link #start()},
 *         {@link #stop()}, {@link #seekTo(int)}, {@link #prepare()} or
 *         {@link #prepareAsync()} in the <em>Idle</em> state for both cases. If any of these
 *         methods is called right after a MediaPlayer object is constructed,
 *         the user supplied callback method OnErrorListener.onError() won't be
 *         called by the internal player engine and the object state remains
 *         unchanged; but if these methods are called right after {@link #reset()},
 *         the user supplied callback method OnErrorListener.onError() will be
 *         invoked by the internal player engine and the object will be
 *         transfered to the <em>Error</em> state. </li>
 *         <li>It is also recommended that once
 *         a MediaPlayer object is no longer being used, call {@link #release()} immediately
 *         so that resources used by the internal player engine associated with the
 *         MediaPlayer object can be released immediately. Resource may include
 *         singleton resources such as hardware acceleration components and
 *         failure to call {@link #release()} may cause subsequent instances of
 *         MediaPlayer objects to fallback to software implementations or fail
 *         altogether. Once the MediaPlayer
 *         object is in the <em>End</em> state, it can no longer be used and
 *         there is no way to bring it back to any other state. </li>
 *         <li>Furthermore,
 *         the MediaPlayer objects created using <code>new</code> is in the
 *         <em>Idle</em> state, while those created with one
 *         of the overloaded convenient <code>create</code> methods are <em>NOT</em>
 *         in the <em>Idle</em> state. In fact, the objects are in the <em>Prepared</em>
 *         state if the creation using <code>create</code> method is successful.
 *         </li>
 *         </ul>
 *         </li>
 *     <li>In general, some playback control operation may fail due to various
 *         reasons, such as unsupported audio/video format, poorly interleaved
 *         audio/video, resolution too high, streaming timeout, and the like.
 *         Thus, error reporting and recovery is an important concern under
 *         these circumstances. Sometimes, due to programming errors, invoking a playback
 *         control operation in an invalid state may also occur. Under all these
 *         error conditions, the internal player engine invokes a user supplied
 *         OnErrorListener.onError() method if an OnErrorListener has been
 *         registered beforehand via
 *         {@link #setOnErrorListener(android.media.MediaPlayer.OnErrorListener)}.
 *         <ul>
 *         <li>It is important to note that once an error occurs, the
 *         MediaPlayer object enters the <em>Error</em> state (except as noted
 *         above), even if an error listener has not been registered by the application.</li>
 *         <li>In order to reuse a MediaPlayer object that is in the <em>
 *         Error</em> state and recover from the error,
 *         {@link #reset()} can be called to restore the object to its <em>Idle</em>
 *         state.</li>
 *         <li>It is good programming practice to have your application
 *         register a OnErrorListener to look out for error notifications from
 *         the internal player engine.</li>
 *         <li>IllegalStateException is
 *         thrown to prevent programming errors such as calling {@link #prepare()},
 *         {@link #prepareAsync()}, or one of the overloaded <code>setDataSource
 *         </code> methods in an invalid state. </li>
 *         </ul>
 *         </li>
 *     <li>Calling
 *         {@link #setDataSource(FileDescriptor)}, or
 *         {@link #setDataSource(String)}, or
 *         {@link #setDataSource(Context, Uri)}, or
 *         {@link #setDataSource(FileDescriptor, long, long)}, or
 *         {@link #setDataSource(MediaDataSource)} transfers a
 *         MediaPlayer object in the <em>Idle</em> state to the
 *         <em>Initialized</em> state.
 *         <ul>
 *         <li>An IllegalStateException is thrown if
 *         setDataSource() is called in any other state.</li>
 *         <li>It is good programming
 *         practice to always look out for <code>IllegalArgumentException</code>
 *         and <code>IOException</code> that may be thrown from the overloaded
 *         <code>setDataSource</code> methods.</li>
 *         </ul>
 *         </li>
 *     <li>A MediaPlayer object must first enter the <em>Prepared</em> state
 *         before playback can be started.
 *         <ul>
 *         <li>There are two ways (synchronous vs.
 *         asynchronous) that the <em>Prepared</em> state can be reached:
 *         either a call to {@link #prepare()} (synchronous) which
 *         transfers the object to the <em>Prepared</em> state once the method call
 *         returns, or a call to {@link #prepareAsync()} (asynchronous) which
 *         first transfers the object to the <em>Preparing</em> state after the
 *         call returns (which occurs almost right way) while the internal
 *         player engine continues working on the rest of preparation work
 *         until the preparation work completes. When the preparation completes or when {@link #prepare()} call returns,
 *         the internal player engine then calls a user supplied callback method,
 *         onPrepared() of the OnPreparedListener interface, if an
 *         OnPreparedListener is registered beforehand via {@link
 *         #setOnPreparedListener(android.media.MediaPlayer.OnPreparedListener)}.</li>
 *         <li>It is important to note that
 *         the <em>Preparing</em> state is a transient state, and the behavior
 *         of calling any method with side effect while a MediaPlayer object is
 *         in the <em>Preparing</em> state is undefined.</li>
 *         <li>An IllegalStateException is
 *         thrown if {@link #prepare()} or {@link #prepareAsync()} is called in
 *         any other state.</li>
 *         <li>While in the <em>Prepared</em> state, properties
 *         such as audio/sound volume, screenOnWhilePlaying, looping can be
 *         adjusted by invoking the corresponding set methods.</li>
 *         </ul>
 *         </li>
 *     <li>To start the playback, {@link #start()} must be called. After
 *         {@link #start()} returns successfully, the MediaPlayer object is in the
 *         <em>Started</em> state. {@link #isPlaying()} can be called to test
 *         whether the MediaPlayer object is in the <em>Started</em> state.
 *         <ul>
 *         <li>While in the <em>Started</em> state, the internal player engine calls
 *         a user supplied OnBufferingUpdateListener.onBufferingUpdate() callback
 *         method if a OnBufferingUpdateListener has been registered beforehand
 *         via {@link #setOnBufferingUpdateListener(OnBufferingUpdateListener)}.
 *         This callback allows applications to keep track of the buffering status
 *         while streaming audio/video.</li>
 *         <li>Calling {@link #start()} has not effect
 *         on a MediaPlayer object that is already in the <em>Started</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>Playback can be paused and stopped, and the current playback position
 *         can be adjusted. Playback can be paused via {@link #pause()}. When the call to
 *         {@link #pause()} returns, the MediaPlayer object enters the
 *         <em>Paused</em> state. Note that the transition from the <em>Started</em>
 *         state to the <em>Paused</em> state and vice versa happens
 *         asynchronously in the player engine. It may take some time before
 *         the state is updated in calls to {@link #isPlaying()}, and it can be
 *         a number of seconds in the case of streamed content.
 *         <ul>
 *         <li>Calling {@link #start()} to resume playback for a paused
 *         MediaPlayer object, and the resumed playback
 *         position is the same as where it was paused. When the call to
 *         {@link #start()} returns, the paused MediaPlayer object goes back to
 *         the <em>Started</em> state.</li>
 *         <li>Calling {@link #pause()} has no effect on
 *         a MediaPlayer object that is already in the <em>Paused</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>Calling  {@link #stop()} stops playback and causes a
 *         MediaPlayer in the <em>Started</em>, <em>Paused</em>, <em>Prepared
 *         </em> or <em>PlaybackCompleted</em> state to enter the
 *         <em>Stopped</em> state.
 *         <ul>
 *         <li>Once in the <em>Stopped</em> state, playback cannot be started
 *         until {@link #prepare()} or {@link #prepareAsync()} are called to set
 *         the MediaPlayer object to the <em>Prepared</em> state again.</li>
 *         <li>Calling {@link #stop()} has no effect on a MediaPlayer
 *         object that is already in the <em>Stopped</em> state.</li>
 *         </ul>
 *         </li>
 *     <li>The playback position can be adjusted with a call to
 *         {@link #seekTo(int)}.
 *         <ul>
 *         <li>Although the asynchronuous {@link #seekTo(int)}
 *         call returns right way, the actual seek operation may take a while to
 *         finish, especially for audio/video being streamed. When the actual
 *         seek operation completes, the internal player engine calls a user
 *         supplied OnSeekComplete.onSeekComplete() if an OnSeekCompleteListener
 *         has been registered beforehand via
 *         {@link #setOnSeekCompleteListener(OnSeekCompleteListener)}.</li>
 *         <li>Please
 *         note that {@link #seekTo(int)} can also be called in the other states,
 *         such as <em>Prepared</em>, <em>Paused</em> and <em>PlaybackCompleted
 *         </em> state.</li>
 *         <li>Furthermore, the actual current playback position
 *         can be retrieved with a call to {@link #getCurrentPosition()}, which
 *         is helpful for applications such as a Music player that need to keep
 *         track of the playback progress.</li>
 *         </ul>
 *         </li>
 *     <li>When the playback reaches the end of stream, the playback completes.
 *         <ul>
 *         <li>If the looping mode was being set to <var>true</var>with
 *         {@link #setLooping(boolean)}, the MediaPlayer object shall remain in
 *         the <em>Started</em> state.</li>
 *         <li>If the looping mode was set to <var>false
 *         </var>, the player engine calls a user supplied callback method,
 *         OnCompletion.onCompletion(), if a OnCompletionListener is registered
 *         beforehand via {@link #setOnCompletionListener(OnCompletionListener)}.
 *         The invoke of the callback signals that the object is now in the <em>
 *         PlaybackCompleted</em> state.</li>
 *         <li>While in the <em>PlaybackCompleted</em>
 *         state, calling {@link #start()} can restart the playback from the
 *         beginning of the audio/video source.</li>
 * </ul>
 *
 *
 * <a name="Valid_and_Invalid_States"></a>
 * <h3>Valid and invalid states</h3>
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr><td>Method Name </p></td>
 *     <td>Valid Sates </p></td>
 *     <td>Invalid States </p></td>
 *     <td>Comments </p></td></tr>
 * <tr><td>attachAuxEffect </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted} </p></td>
 *     <td>{Idle, Error} </p></td>
 *     <td>This method must be called after setDataSource.
 *     Calling it does not change the object state. </p></td></tr>
 * <tr><td>getAudioSessionId </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>getCurrentPosition </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted} </p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state. </p></td></tr>
 * <tr><td>getDuration </p></td>
 *     <td>{Prepared, Started, Paused, Stopped, PlaybackCompleted} </p></td>
 *     <td>{Idle, Initialized, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state. </p></td></tr>
 * <tr><td>getVideoHeight </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change the
 *         state. Calling this method in an invalid state transfers the object
 *         to the <em>Error</em> state.  </p></td></tr>
 * <tr><td>getVideoWidth </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>isPlaying </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>pause </p></td>
 *     <td>{Started, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Prepared, Stopped, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Paused</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>prepare </p></td>
 *     <td>{Initialized, Stopped} </p></td>
 *     <td>{Idle, Prepared, Started, Paused, PlaybackCompleted, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Prepared</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>prepareAsync </p></td>
 *     <td>{Initialized, Stopped} </p></td>
 *     <td>{Idle, Prepared, Started, Paused, PlaybackCompleted, Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Preparing</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>release </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>After {@link #release()}, the object is no longer available. </p></td></tr>
 * <tr><td>reset </p></td>
 *     <td>{Idle, Initialized, Prepared, Started, Paused, Stopped,
 *         PlaybackCompleted, Error}</p></td>
 *     <td>{}</p></td>
 *     <td>After {@link #reset()}, the object is like being just created.</p></td></tr>
 * <tr><td>seekTo </p></td>
 *     <td>{Prepared, Started, Paused, PlaybackCompleted} </p></td>
 *     <td>{Idle, Initialized, Stopped, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an invalid state transfers the
 *         object to the <em>Error</em> state. </p></td></tr>
 * <tr><td>setAudioAttributes </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method does not change the state. In order for the
 *         target audio attributes type to become effective, this method must be called before
 *         prepare() or prepareAsync().</p></td></tr>
 * <tr><td>setAudioSessionId </p></td>
 *     <td>{Idle} </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted,
 *          Error} </p></td>
 *     <td>This method must be called in idle state as the audio session ID must be known before
 *         calling setDataSource. Calling it does not change the object state. </p></td></tr>
 * <tr><td>setAudioStreamType </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method does not change the state. In order for the
 *         target audio stream type to become effective, this method must be called before
 *         prepare() or prepareAsync().</p></td></tr>
 * <tr><td>setAuxEffectSendLevel </p></td>
 *     <td>any</p></td>
 *     <td>{} </p></td>
 *     <td>Calling this method does not change the object state. </p></td></tr>
 * <tr><td>setDataSource </p></td>
 *     <td>{Idle} </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted,
 *          Error} </p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Initialized</em> state. Calling this method in an
 *         invalid state throws an IllegalStateException.</p></td></tr>
 * <tr><td>setDisplay </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setSurface </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setVideoScalingMode </p></td>
 *     <td>{Initialized, Prepared, Started, Paused, Stopped, PlaybackCompleted} </p></td>
 *     <td>{Idle, Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.</p></td></tr>
 * <tr><td>setLooping </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *         PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method in a valid state does not change
 *         the state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>isLooping </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnBufferingUpdateListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnCompletionListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnErrorListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnPreparedListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setOnSeekCompleteListener </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state. </p></td></tr>
 * <tr><td>setPlaybackParams</p></td>
 *     <td>{Initialized, Prepared, Started, Paused, PlaybackCompleted, Error}</p></td>
 *     <td>{Idle, Stopped} </p></td>
 *     <td>This method will change state in some cases, depending on when it's called.
 *         </p></td></tr>
 * <tr><td>setScreenOnWhilePlaying</></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state.  </p></td></tr>
 * <tr><td>setVolume </p></td>
 *     <td>{Idle, Initialized, Stopped, Prepared, Started, Paused,
 *          PlaybackCompleted}</p></td>
 *     <td>{Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.
 * <tr><td>setWakeMode </p></td>
 *     <td>any </p></td>
 *     <td>{} </p></td>
 *     <td>This method can be called in any state and calling it does not change
 *         the object state.</p></td></tr>
 * <tr><td>start </p></td>
 *     <td>{Prepared, Started, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Stopped, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Started</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>stop </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method in a valid state transfers the
 *         object to the <em>Stopped</em> state. Calling this method in an
 *         invalid state transfers the object to the <em>Error</em> state.</p></td></tr>
 * <tr><td>getTrackInfo </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.</p></td></tr>
 * <tr><td>addTimedTextSource </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.</p></td></tr>
 * <tr><td>selectTrack </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.</p></td></tr>
 * <tr><td>deselectTrack </p></td>
 *     <td>{Prepared, Started, Stopped, Paused, PlaybackCompleted}</p></td>
 *     <td>{Idle, Initialized, Error}</p></td>
 *     <td>Successful invoke of this method does not change the state.</p></td></tr>
 *
 * </table>
 *
 * <a name="Permissions"></a>
 * <h3>Permissions</h3>
 * <p>One may need to declare a corresponding WAKE_LOCK permission {@link
 * android.R.styleable#AndroidManifestUsesPermission &lt;uses-permission&gt;}
 * element.
 *
 * <p>This class requires the {@link android.Manifest.permission#INTERNET} permission
 * when used with network-based content.
 *
 * <a name="Callbacks"></a>
 * <h3>Callbacks</h3>
 * <p>Applications may want to register for informational and error
 * events in order to be informed of some internal state update and
 * possible runtime errors during playback or streaming. Registration for
 * these events is done by properly setting the appropriate listeners (via calls
 * to
 * {@link #setOnPreparedListener(OnPreparedListener)}setOnPreparedListener,
 * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}setOnVideoSizeChangedListener,
 * {@link #setOnSeekCompleteListener(OnSeekCompleteListener)}setOnSeekCompleteListener,
 * {@link #setOnCompletionListener(OnCompletionListener)}setOnCompletionListener,
 * {@link #setOnBufferingUpdateListener(OnBufferingUpdateListener)}setOnBufferingUpdateListener,
 * {@link #setOnInfoListener(OnInfoListener)}setOnInfoListener,
 * {@link #setOnErrorListener(OnErrorListener)}setOnErrorListener, etc).
 * In order to receive the respective callback
 * associated with these listeners, applications are required to create
 * MediaPlayer objects on a thread with its own Looper running (main UI
 * thread by default has a Looper running).
 *
 */
public class MediaPlayer extends PlayerBase
                         implements SubtitleController.Listener
{
    /**
       Constant to retrieve only the new metadata since the last
       call.
       // FIXME: unhide.
       // FIXME: add link to getMetadata(boolean, boolean)
       {@hide}
     */
    public static final boolean METADATA_UPDATE_ONLY = true;

    /**
       Constant to retrieve all the metadata.
       // FIXME: unhide.
       // FIXME: add link to getMetadata(boolean, boolean)
       {@hide}
     */
    public static final boolean METADATA_ALL = false;

    /**
       Constant to enable the metadata filter during retrieval.
       // FIXME: unhide.
       // FIXME: add link to getMetadata(boolean, boolean)
       {@hide}
     */
    public static final boolean APPLY_METADATA_FILTER = true;

    /**
       Constant to disable the metadata filter during retrieval.
       // FIXME: unhide.
       // FIXME: add link to getMetadata(boolean, boolean)
       {@hide}
     */
    public static final boolean BYPASS_METADATA_FILTER = false;

    public static final int FORCE_NONE= 0;
    public static final int FORCE_ANDROID_MEDIAPLAYER = 1;
    public static final int FORCE_RT_MEDIAPLAYER = 2;
    public static final int FORCE_BY_SCORE = 3;

    public static final int KEY_PARAMETER_VIDEO_BUFFER_SIZE = 2000;
    public static final int KEY_PARAMETER_AUDIO_BALANCE = 2001;

    public static final int AUDIO_BALANCE_VOCAL_REMOVAL_NONE = 0;
    public static final int AUDIO_BALANCE_VOCAL_REMOVAL_LEFT_CHANNEL = 1;
    public static final int AUDIO_BALANCE_VOCAL_REMOVAL_RIGHT_CHANNEL = 2;
    public static final int AUDIO_BALANCE_VOCAL_REMOVAL_LEFT_RIGHT_DIFFERENCE = 3;
    public static final int AUDIO_BALANCE_VOCAL_REMOVAL_ADVANCED = 4;

    static {
        System.loadLibrary("media_jni");
        native_init();
    }

    private final static String TAG = "MediaPlayer";
    // Name of the remote interface for the media player. Must be kept
    // in sync with the 2nd parameter of the IMPLEMENT_META_INTERFACE
    // macro invocation in IMediaPlayer.cpp
    private final static String IMEDIA_PLAYER = "android.media.IMediaPlayer";

    private long mNativeContext; // accessed by native methods
    private long mNativeSurfaceTexture;  // accessed by native methods
    private int mListenerContext; // accessed by native methods
    private SurfaceHolder mSurfaceHolder;
    private EventHandler mEventHandler;
    private PowerManager.WakeLock mWakeLock = null;
    private boolean mScreenOnWhilePlaying;
    private boolean mStayAwake;
    private int mStreamType = AudioManager.USE_DEFAULT_STREAM_TYPE;
    private int mUsage = -1;
    private boolean mBypassInterruptionPolicy;

    // added by RTK
    private boolean ENABLE_RT_MEDIAPLAYER = false;
    private String RTMediaPlayer_PropValue = null;
    private Context mContext = null;
    // end of added by RTK

    /**
     * Default constructor. Consider using one of the create() methods for
     * synchronously instantiating a MediaPlayer from a Uri or resource.
     * <p>When done with the MediaPlayer, you should call  {@link #release()},
     * to free the resources. If not released, too many MediaPlayer instances may
     * result in an exception.</p>
     */
    public MediaPlayer() {
        super(new AudioAttributes.Builder().build());

        // added by RTK
        RTMediaPlayer_PropValue = SystemProperties.get("persist.media.USE_RTMediaPlayer", "false");
        if (RTMediaPlayer_PropValue.equals("true") || RTMediaPlayer_PropValue.equals("1"))
            ENABLE_RT_MEDIAPLAYER = true;
        mContext = null;
        // end of added by RTK

        Looper looper;
        if ((looper = Looper.myLooper()) != null) {
            mEventHandler = new EventHandler(this, looper);
        } else if ((looper = Looper.getMainLooper()) != null) {
            mEventHandler = new EventHandler(this, looper);
        } else {
            mEventHandler = null;
        }

        mTimeProvider = new TimeProvider(this);
        mOpenSubtitleSources = new Vector<InputStream>();

        /* Native setup requires a weak reference to our object.
         * It's easier to create it here than in C++.
         */
        native_setup(new WeakReference<MediaPlayer>(this));
    }

    /*
     * Update the MediaPlayer SurfaceTexture.
     * Call after setting a new display surface.
     */
    private native void _setVideoSurface(Surface surface);

    /* Do not change these values (starting with INVOKE_ID) without updating
     * their counterparts in include/media/mediaplayer.h!
     */
    private static final int INVOKE_ID_GET_TRACK_INFO = 1;
    private static final int INVOKE_ID_ADD_EXTERNAL_SOURCE = 2;
    private static final int INVOKE_ID_ADD_EXTERNAL_SOURCE_FD = 3;
    private static final int INVOKE_ID_SELECT_TRACK = 4;
    private static final int INVOKE_ID_DESELECT_TRACK = 5;
    private static final int INVOKE_ID_SET_VIDEO_SCALE_MODE = 6;
    private static final int INVOKE_ID_GET_SELECTED_TRACK = 7;
    // added by RTK
    private static final int INVOKE_ID_SET_TV_SYSTEM_TYPE = 8;
    private static final int INVOKE_ID_SET_LANGUAGES = 9;
    private static final int INVOKE_ID_SET_GET_NAV_PROPERTY = 10;
    private static final int INVOKE_ID_EXEC_BOOKMARK_OPERATION = 11;
    private static final int INVOKE_ID_EXEC_SUBTITLE_OPERATION = 12;
    private static final int INVOKE_ID_EXEC_AUDIO_OPERATION = 13;
    private static final int INVOKE_ID_SET_VIDEO_SCALED_SIZE = 14;
    // end of RTK

    /**
     * Create a request parcel which can be routed to the native media
     * player using {@link #invoke(Parcel, Parcel)}. The Parcel
     * returned has the proper InterfaceToken set. The caller should
     * not overwrite that token, i.e it can only append data to the
     * Parcel.
     *
     * @return A parcel suitable to hold a request for the native
     * player.
     * {@hide}
     */
    public Parcel newRequest() {
        Parcel parcel = Parcel.obtain();
        parcel.writeInterfaceToken(IMEDIA_PLAYER);
        return parcel;
    }

    /**
     * Invoke a generic method on the native player using opaque
     * parcels for the request and reply. Both payloads' format is a
     * convention between the java caller and the native player.
     * Must be called after setDataSource to make sure a native player
     * exists. On failure, a RuntimeException is thrown.
     *
     * @param request Parcel with the data for the extension. The
     * caller must use {@link #newRequest()} to get one.
     *
     * @param reply Output parcel with the data returned by the
     * native player.
     * {@hide}
     */
    public void invoke(Parcel request, Parcel reply) {
        int retcode = native_invoke(request, reply);
        reply.setDataPosition(0);
        if (retcode != 0) {
            throw new RuntimeException("failure code: " + retcode);
        }
    }

    /**
     * Sets the {@link SurfaceHolder} to use for displaying the video
     * portion of the media.
     *
     * Either a surface holder or surface must be set if a display or video sink
     * is needed.  Not calling this method or {@link #setSurface(Surface)}
     * when playing back a video will result in only the audio track being played.
     * A null surface holder or surface will result in only the audio track being
     * played.
     *
     * @param sh the SurfaceHolder to use for video display
     * @throws IllegalStateException if the internal player engine has not been
     * initialized or has been released.
     */
    public void setDisplay(SurfaceHolder sh) {
        mSurfaceHolder = sh;
        Surface surface;
        if (sh != null) {
            surface = sh.getSurface();
        } else {
            surface = null;
        }
        _setVideoSurface(surface);
        updateSurfaceScreenOn();
    }

    /**
     * Sets the {@link Surface} to be used as the sink for the video portion of
     * the media. This is similar to {@link #setDisplay(SurfaceHolder)}, but
     * does not support {@link #setScreenOnWhilePlaying(boolean)}.  Setting a
     * Surface will un-set any Surface or SurfaceHolder that was previously set.
     * A null surface will result in only the audio track being played.
     *
     * If the Surface sends frames to a {@link SurfaceTexture}, the timestamps
     * returned from {@link SurfaceTexture#getTimestamp()} will have an
     * unspecified zero point.  These timestamps cannot be directly compared
     * between different media sources, different instances of the same media
     * source, or multiple runs of the same program.  The timestamp is normally
     * monotonically increasing and is unaffected by time-of-day adjustments,
     * but it is reset when the position is set.
     *
     * @param surface The {@link Surface} to be used for the video portion of
     * the media.
     * @throws IllegalStateException if the internal player engine has not been
     * initialized or has been released.
     */
    public void setSurface(Surface surface) {
        if (mScreenOnWhilePlaying && surface != null) {
            Log.w(TAG, "setScreenOnWhilePlaying(true) is ineffective for Surface");
        }
        mSurfaceHolder = null;
        _setVideoSurface(surface);
        updateSurfaceScreenOn();
    }

    /* Do not change these video scaling mode values below without updating
     * their counterparts in system/window.h! Please do not forget to update
     * {@link #isVideoScalingModeSupported} when new video scaling modes
     * are added.
     */
    /**
     * Specifies a video scaling mode. The content is stretched to the
     * surface rendering area. When the surface has the same aspect ratio
     * as the content, the aspect ratio of the content is maintained;
     * otherwise, the aspect ratio of the content is not maintained when video
     * is being rendered. Unlike {@link #VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING},
     * there is no content cropping with this video scaling mode.
     */
    public static final int VIDEO_SCALING_MODE_SCALE_TO_FIT = 1;

    /**
     * Specifies a video scaling mode. The content is scaled, maintaining
     * its aspect ratio. The whole surface area is always used. When the
     * aspect ratio of the content is the same as the surface, no content
     * is cropped; otherwise, content is cropped to fit the surface.
     */
    public static final int VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING = 2;
    /**
     * Sets video scaling mode. To make the target video scaling mode
     * effective during playback, this method must be called after
     * data source is set. If not called, the default video
     * scaling mode is {@link #VIDEO_SCALING_MODE_SCALE_TO_FIT}.
     *
     * <p> The supported video scaling modes are:
     * <ul>
     * <li> {@link #VIDEO_SCALING_MODE_SCALE_TO_FIT}
     * <li> {@link #VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING}
     * </ul>
     *
     * @param mode target video scaling mode. Must be one of the supported
     * video scaling modes; otherwise, IllegalArgumentException will be thrown.
     *
     * @see MediaPlayer#VIDEO_SCALING_MODE_SCALE_TO_FIT
     * @see MediaPlayer#VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING
     */
    public void setVideoScalingMode(int mode) {
        if (!isVideoScalingModeSupported(mode)) {
            final String msg = "Scaling mode " + mode + " is not supported";
            throw new IllegalArgumentException(msg);
        }
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_VIDEO_SCALE_MODE);
            request.writeInt(mode);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Convenience method to create a MediaPlayer for a given Uri.
     * On success, {@link #prepare()} will already have been called and must not be called again.
     * <p>When done with the MediaPlayer, you should call  {@link #release()},
     * to free the resources. If not released, too many MediaPlayer instances will
     * result in an exception.</p>
     * <p>Note that since {@link #prepare()} is called automatically in this method,
     * you cannot change the audio stream type (see {@link #setAudioStreamType(int)}), audio
     * session ID (see {@link #setAudioSessionId(int)}) or audio attributes
     * (see {@link #setAudioAttributes(AudioAttributes)} of the new MediaPlayer.</p>
     *
     * @param context the Context to use
     * @param uri the Uri from which to get the datasource
     * @return a MediaPlayer object, or null if creation failed
     */
    public static MediaPlayer create(Context context, Uri uri) {
        return create (context, uri, null);
    }

    /**
     * Convenience method to create a MediaPlayer for a given Uri.
     * On success, {@link #prepare()} will already have been called and must not be called again.
     * <p>When done with the MediaPlayer, you should call  {@link #release()},
     * to free the resources. If not released, too many MediaPlayer instances will
     * result in an exception.</p>
     * <p>Note that since {@link #prepare()} is called automatically in this method,
     * you cannot change the audio stream type (see {@link #setAudioStreamType(int)}), audio
     * session ID (see {@link #setAudioSessionId(int)}) or audio attributes
     * (see {@link #setAudioAttributes(AudioAttributes)} of the new MediaPlayer.</p>
     *
     * @param context the Context to use
     * @param uri the Uri from which to get the datasource
     * @param holder the SurfaceHolder to use for displaying the video
     * @return a MediaPlayer object, or null if creation failed
     */
    public static MediaPlayer create(Context context, Uri uri, SurfaceHolder holder) {
        int s = AudioSystem.newAudioSessionId();
        return create(context, uri, holder, null, s > 0 ? s : 0);
    }

    /**
     * Same factory method as {@link #create(Context, Uri, SurfaceHolder)} but that lets you specify
     * the audio attributes and session ID to be used by the new MediaPlayer instance.
     * @param context the Context to use
     * @param uri the Uri from which to get the datasource
     * @param holder the SurfaceHolder to use for displaying the video, may be null.
     * @param audioAttributes the {@link AudioAttributes} to be used by the media player.
     * @param audioSessionId the audio session ID to be used by the media player,
     *     see {@link AudioManager#generateAudioSessionId()} to obtain a new session.
     * @return a MediaPlayer object, or null if creation failed
     */
    public static MediaPlayer create(Context context, Uri uri, SurfaceHolder holder,
            AudioAttributes audioAttributes, int audioSessionId) {

        try {
            MediaPlayer mp = new MediaPlayer();
            final AudioAttributes aa = audioAttributes != null ? audioAttributes :
                new AudioAttributes.Builder().build();
            mp.setAudioAttributes(aa);
            mp.setAudioSessionId(audioSessionId);
            mp.setDataSource(context, uri);
            if (holder != null) {
                mp.setDisplay(holder);
            }
            mp.prepare();
            return mp;
        } catch (IOException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        } catch (IllegalArgumentException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        } catch (SecurityException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        }

        return null;
    }

    // Note no convenience method to create a MediaPlayer with SurfaceTexture sink.

    /**
     * Convenience method to create a MediaPlayer for a given resource id.
     * On success, {@link #prepare()} will already have been called and must not be called again.
     * <p>When done with the MediaPlayer, you should call  {@link #release()},
     * to free the resources. If not released, too many MediaPlayer instances will
     * result in an exception.</p>
     * <p>Note that since {@link #prepare()} is called automatically in this method,
     * you cannot change the audio stream type (see {@link #setAudioStreamType(int)}), audio
     * session ID (see {@link #setAudioSessionId(int)}) or audio attributes
     * (see {@link #setAudioAttributes(AudioAttributes)} of the new MediaPlayer.</p>
     *
     * @param context the Context to use
     * @param resid the raw resource id (<var>R.raw.&lt;something></var>) for
     *              the resource to use as the datasource
     * @return a MediaPlayer object, or null if creation failed
     */
    public static MediaPlayer create(Context context, int resid) {
        int s = AudioSystem.newAudioSessionId();
        return create(context, resid, null, s > 0 ? s : 0);
    }

    /**
     * Same factory method as {@link #create(Context, int)} but that lets you specify the audio
     * attributes and session ID to be used by the new MediaPlayer instance.
     * @param context the Context to use
     * @param resid the raw resource id (<var>R.raw.&lt;something></var>) for
     *              the resource to use as the datasource
     * @param audioAttributes the {@link AudioAttributes} to be used by the media player.
     * @param audioSessionId the audio session ID to be used by the media player,
     *     see {@link AudioManager#generateAudioSessionId()} to obtain a new session.
     * @return a MediaPlayer object, or null if creation failed
     */
    public static MediaPlayer create(Context context, int resid,
            AudioAttributes audioAttributes, int audioSessionId) {
        try {
            AssetFileDescriptor afd = context.getResources().openRawResourceFd(resid);
            if (afd == null) return null;

            MediaPlayer mp = new MediaPlayer();

            final AudioAttributes aa = audioAttributes != null ? audioAttributes :
                new AudioAttributes.Builder().build();
            mp.setAudioAttributes(aa);
            mp.setAudioSessionId(audioSessionId);

            mp.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
            afd.close();
            mp.prepare();
            return mp;
        } catch (IOException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        } catch (IllegalArgumentException ex) {
            Log.d(TAG, "create failed:", ex);
           // fall through
        } catch (SecurityException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        }
        return null;
    }

    /**
     * Sets the data source as a content Uri.
     *
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @throws IllegalStateException if it is called in an invalid state
     */
    public void setDataSource(@NonNull Context context, @NonNull Uri uri)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        setDataSource(context, uri, null);
    }

    /**
     * Sets the data source as a content Uri.
     *
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @param headers the headers to be sent together with the request for the data
     *                Note that the cross domain redirection is allowed by default, but that can be
     *                changed with key/value pairs through the headers parameter with
     *                "android-allow-cross-domain-redirect" as the key and "0" or "1" as the value
     *                to disallow or allow cross domain redirection.
     * @throws IllegalStateException if it is called in an invalid state
     */
    public void setDataSource(@NonNull Context context, @NonNull Uri uri,
            @Nullable Map<String, String> headers) throws IOException, IllegalArgumentException,
                    SecurityException, IllegalStateException {
        //not local
        if (context!=null) {
        	mContext = context;
        	Log.i(TAG, "setDataSource,WFD OFF");
        	Intent i = new Intent("rtk.provider.miracastsink.STATUS");
			i.putExtra("WFDMode", "WFD_OFF");
			context.sendBroadcast(i);
			/*try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				Log.i(TAG, "setDataSource,WFD OFF exception.");
			}*/
			Log.i(TAG, "setDataSource,WFD OFF 1");
		}

        final ContentResolver resolver = context.getContentResolver();
        final String scheme = uri.getScheme();
        if (ContentResolver.SCHEME_FILE.equals(scheme)) {
            setDataSource(uri.toString());
            return;
        } else if (ContentResolver.SCHEME_CONTENT.equals(scheme)
                && Settings.AUTHORITY.equals(uri.getAuthority())) {
            // Try cached ringtone first since the actual provider may not be
            // encryption aware, or it may be stored on CE media storage
            final int type = RingtoneManager.getDefaultType(uri);
            final Uri cacheUri = RingtoneManager.getCacheForType(type);
            final Uri actualUri = RingtoneManager.getActualDefaultRingtoneUri(context, type);
            if (attemptDataSource(resolver, cacheUri)) {
                return;
            } else if (attemptDataSource(resolver, actualUri)) {
                return;
            } else {
                setDataSource(uri.toString(), headers);
            }
        } else {
            // Try requested Uri locally first, or fallback to media server
            if (attemptDataSource(resolver, uri)) {
                return;
            } else {
                setDataSource(uri.toString(), headers);
            }
        }
    }

    private boolean attemptDataSource(ContentResolver resolver, Uri uri) {
        try (AssetFileDescriptor afd = resolver.openAssetFileDescriptor(uri, "r")) {
            setDataSource(afd);
            return true;
        } catch (NullPointerException | SecurityException | IOException ex) {
            Log.w(TAG, "Couldn't open " + uri + ": " + ex);
            return false;
        }
    }

    /**
     * Sets the data source (file-path or http/rtsp URL) to use.
     *
     * @param path the path of the file, or the http/rtsp URL of the stream you want to play
     * @throws IllegalStateException if it is called in an invalid state
     *
     * <p>When <code>path</code> refers to a local file, the file may actually be opened by a
     * process other than the calling application.  This implies that the pathname
     * should be an absolute path (as any other process runs with unspecified current working
     * directory), and that the pathname should reference a world-readable file.
     * As an alternative, the application could first open the file for reading,
     * and then use the file descriptor form {@link #setDataSource(FileDescriptor)}.
     */
    public void setDataSource(String path)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        setDataSource(path, null, null);
    }

    /**
     * Sets the data source (file-path or http/rtsp URL) to use.
     *
     * @param path the path of the file, or the http/rtsp URL of the stream you want to play
     * @param headers the headers associated with the http request for the stream you want to play
     * @throws IllegalStateException if it is called in an invalid state
     * @hide pending API council
     */
    public void setDataSource(String path, Map<String, String> headers)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException
    {
        String[] keys = null;
        String[] values = null;

        if (headers != null) {
            keys = new String[headers.size()];
            values = new String[headers.size()];

            int i = 0;
            for (Map.Entry<String, String> entry: headers.entrySet()) {
                keys[i] = entry.getKey();
                values[i] = entry.getValue();
                ++i;
            }
        }
        setDataSource(path, keys, values);
    }

    private void setDataSource(String path, String[] keys, String[] values)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        final Uri uri = Uri.parse(path);
        final String scheme = uri.getScheme();
        //Log.v(TAG, "setDataSource, coming path = " + path);
        //Log.v(TAG, "setDataSource, scheme = " + scheme);
        if ("file".equals(scheme)) {
            //path = uri.getPath();
            String fragment = uri.getFragment();

            path = uri.getSchemeSpecificPart();
            path = path.substring(2); // skip the first 2 characters.
            if (fragment != null)
                path = path + "#" + fragment;
        } else if (scheme != null) {
            // added by RTK
            if (("udp".equals(scheme)) || ("rtp".equals(scheme)) || ("mms".equals(scheme)) || ("mmst".equals(scheme)) || ("mmsh".equals(scheme)) || ("rtmp".equals(scheme)))
            {
                Log.v(TAG, "setDataSource, streaming url = " + path);
                useRTMediaPlayer(FORCE_RT_MEDIAPLAYER);
                if (path.startsWith("udp://@"))
                {
                    path = path.substring(0, 6) + path.substring(7, path.length()); // skip '@' character
                    //Log.v(TAG, "setDataSource, path = " + path);
                }
            }
            // end of added by RTK
            // handle non-file sources
            nativeSetDataSource(
                MediaHTTPService.createHttpServiceBinderIfNecessary(path),
                path,
                keys,
                values);
            return;
        }
        if (path.startsWith("http%3A%2F%2F") == true || path.startsWith("https%3A%2F%2F") == true)
        {
            path = Uri.decode(path);
        }
        Log.v(TAG, "setDataSource, path = " + path);

        final File file = new File(path);
        /*
         * By hamer
         * Fix setDataSource failed issue if url is a folder path.
         *   sol: add the check whether file is a File (file.isFile())
         */
        if (file.isFile() && file.exists()) {
            FileInputStream is = new FileInputStream(file);
            FileDescriptor fd = is.getFD();
            setDataSource(fd);
            is.close();
// added by RTK
// To prevent from permission rule, check the access permission here if using RTMediaPlayer
// because DvdPlayer is run with root so it can always access the file in sdcard such that
// the permission check rule fails.
        } 
        else if (file.isDirectory()) {
            nativeSetDataSource(null, path, keys, values);
        }
        else if ((false == path.startsWith("http://") && false == path.startsWith("https://")
                    && false == path.startsWith("rtsp://")
                    && false == path.startsWith("mms://")
                    && false == path.startsWith("mmst://")
                    && false == path.startsWith("mmsh://")
                    && false == path.startsWith("directconnect://")
                    && false == path.startsWith("stb://")
                    && false == path.startsWith("rtmp://")
                    && false == path.startsWith("fileringarray://")
                    && false == path.startsWith("rtp://")
                    && false == path.startsWith("udp://"))
                && ENABLE_RT_MEDIAPLAYER) {
            throw new IOException("Permission Denied");
// end of RTK
        } else {
            throw new IOException("setDataSource failed.");
        }
    }

    private native void nativeSetDataSource(
        IBinder httpServiceBinder, String path, String[] keys, String[] values)
        throws IOException, IllegalArgumentException, SecurityException, IllegalStateException;

    /**
     * Sets the data source (AssetFileDescriptor) to use. It is the caller's
     * responsibility to close the file descriptor. It is safe to do so as soon
     * as this call returns.
     *
     * @param afd the AssetFileDescriptor for the file you want to play
     * @throws IllegalStateException if it is called in an invalid state
     * @throws IllegalArgumentException if afd is not a valid AssetFileDescriptor
     * @throws IOException if afd can not be read
     */
    public void setDataSource(@NonNull AssetFileDescriptor afd)
            throws IOException, IllegalArgumentException, IllegalStateException {
        Preconditions.checkNotNull(afd);
        // Note: using getDeclaredLength so that our behavior is the same
        // as previous versions when the content provider is returning
        // a full file.
        if (afd.getDeclaredLength() < 0) {
            setDataSource(afd.getFileDescriptor());
        } else {
            setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getDeclaredLength());
        }
    }

    /**
     * Sets the data source (FileDescriptor) to use. It is the caller's responsibility
     * to close the file descriptor. It is safe to do so as soon as this call returns.
     *
     * @param fd the FileDescriptor for the file you want to play
     * @throws IllegalStateException if it is called in an invalid state
     * @throws IllegalArgumentException if fd is not a valid FileDescriptor
     * @throws IOException if fd can not be read
     */
    public void setDataSource(FileDescriptor fd)
            throws IOException, IllegalArgumentException, IllegalStateException {
        // intentionally less than LONG_MAX
        setDataSource(fd, 0, 0x7ffffffffffffffL);
    }

    /**
     * Sets the data source (FileDescriptor) to use.  The FileDescriptor must be
     * seekable (N.B. a LocalSocket is not seekable). It is the caller's responsibility
     * to close the file descriptor. It is safe to do so as soon as this call returns.
     *
     * @param fd the FileDescriptor for the file you want to play
     * @param offset the offset into the file where the data to be played starts, in bytes
     * @param length the length in bytes of the data to be played
     * @throws IllegalStateException if it is called in an invalid state
     * @throws IllegalArgumentException if fd is not a valid FileDescriptor
     * @throws IOException if fd can not be read
     */
    public void setDataSource(FileDescriptor fd, long offset, long length)
            throws IOException, IllegalArgumentException, IllegalStateException {
        _setDataSource(fd, offset, length);
    }

    private native void _setDataSource(FileDescriptor fd, long offset, long length)
            throws IOException, IllegalArgumentException, IllegalStateException;

    /**
     * Sets the data source (MediaDataSource) to use.
     *
     * @param dataSource the MediaDataSource for the media you want to play
     * @throws IllegalStateException if it is called in an invalid state
     * @throws IllegalArgumentException if dataSource is not a valid MediaDataSource
     */
    public void setDataSource(MediaDataSource dataSource)
            throws IllegalArgumentException, IllegalStateException {
        _setDataSource(dataSource);
    }

    private native void _setDataSource(MediaDataSource dataSource)
          throws IllegalArgumentException, IllegalStateException;

    /**
     * Prepares the player for playback, synchronously.
     *
     * After setting the datasource and the display surface, you need to either
     * call prepare() or prepareAsync(). For files, it is OK to call prepare(),
     * which blocks until MediaPlayer is ready for playback.
     *
     * @throws IllegalStateException if it is called in an invalid state
     */
    public void prepare() throws IOException, IllegalStateException {
        _prepare();
        scanInternalSubtitleTracks();
    }

    private native void _prepare() throws IOException, IllegalStateException;

    /**
     * Prepares the player for playback, asynchronously.
     *
     * After setting the datasource and the display surface, you need to either
     * call prepare() or prepareAsync(). For streams, you should call prepareAsync(),
     * which returns immediately, rather than blocking until enough data has been
     * buffered.
     *
     * @throws IllegalStateException if it is called in an invalid state
     */
    public native void prepareAsync() throws IllegalStateException;

    /**
     * Starts or resumes playback. If playback had previously been paused,
     * playback will continue from where it was paused. If playback had
     * been stopped, or never started before, playback will start at the
     * beginning.
     *
     * @throws IllegalStateException if it is called in an invalid state
     */
    public void start() throws IllegalStateException {
        baseStart();
        stayAwake(true);
// added by RTK
        // FIXME: find a better place to start the execution of network speed measurement thread
        // such as while buffer underruns. This should untimately be handled in the downloading
        // thread.

        /** The sending of MEDIA_INFO_NETWORK_BANDWIDTH has been moved to RTMediaPlayer.
        if (ENABLE_RT_MEDIAPLAYER) {
            synchronized(this)
            {
                if (null == measure) {
                    measure = new SpeedMeasureThread();
                    Log.d(TAG, "starting SpeedMeasureThread...");
                    measure.start();
                }
            }
        }
        */
// end of added by RTK
        _start();
    }

    private native void _start() throws IllegalStateException;


    private int getAudioStreamType() {
        if (mStreamType == AudioManager.USE_DEFAULT_STREAM_TYPE) {
            mStreamType = _getAudioStreamType();
        }
        return mStreamType;
    }

    private native int _getAudioStreamType() throws IllegalStateException;

    /**
     * Stops playback after playback has been stopped or paused.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    public void stop() throws IllegalStateException {
        stayAwake(false);
        _stop();
    }

    private native void _stop() throws IllegalStateException;

    /**
     * Pauses playback. Call start() to resume.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    public void pause() throws IllegalStateException {
// added by RTK
        if (ENABLE_RT_MEDIAPLAYER) {
            stopMeasureThread();
        }
// end of add by RTK
        stayAwake(false);
        _pause();
    }

    private native void _pause() throws IllegalStateException;

    /**
     * Set the low-level power management behavior for this MediaPlayer.  This
     * can be used when the MediaPlayer is not playing through a SurfaceHolder
     * set with {@link #setDisplay(SurfaceHolder)} and thus can use the
     * high-level {@link #setScreenOnWhilePlaying(boolean)} feature.
     *
     * <p>This function has the MediaPlayer access the low-level power manager
     * service to control the device's power usage while playing is occurring.
     * The parameter is a combination of {@link android.os.PowerManager} wake flags.
     * Use of this method requires {@link android.Manifest.permission#WAKE_LOCK}
     * permission.
     * By default, no attempt is made to keep the device awake during playback.
     *
     * @param context the Context to use
     * @param mode    the power/wake mode to set
     * @see android.os.PowerManager
     */
    public void setWakeMode(Context context, int mode) {
        boolean washeld = false;

        /* Disable persistant wakelocks in media player based on property */
        if (SystemProperties.getBoolean("audio.offload.ignore_setawake", false) == true) {
            Log.w(TAG, "IGNORING setWakeMode " + mode);
            return;
        }

        if (mWakeLock != null) {
            if (mWakeLock.isHeld()) {
                washeld = true;
                mWakeLock.release();
            }
            mWakeLock = null;
        }

        PowerManager pm = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(mode|PowerManager.ON_AFTER_RELEASE, MediaPlayer.class.getName());
        mWakeLock.setReferenceCounted(false);
        if (washeld) {
            mWakeLock.acquire();
        }
    }

    /**
     * Control whether we should use the attached SurfaceHolder to keep the
     * screen on while video playback is occurring.  This is the preferred
     * method over {@link #setWakeMode} where possible, since it doesn't
     * require that the application have permission for low-level wake lock
     * access.
     *
     * @param screenOn Supply true to keep the screen on, false to allow it
     * to turn off.
     */
    public void setScreenOnWhilePlaying(boolean screenOn) {
        if (mScreenOnWhilePlaying != screenOn) {
            if (screenOn && mSurfaceHolder == null) {
                Log.w(TAG, "setScreenOnWhilePlaying(true) is ineffective without a SurfaceHolder");
            }
            mScreenOnWhilePlaying = screenOn;
            updateSurfaceScreenOn();
        }
    }

    private void stayAwake(boolean awake) {
        if (mWakeLock != null) {
            if (awake && !mWakeLock.isHeld()) {
                mWakeLock.acquire();
            } else if (!awake && mWakeLock.isHeld()) {
                mWakeLock.release();
            }
        }
        mStayAwake = awake;
        updateSurfaceScreenOn();
    }

    private void updateSurfaceScreenOn() {
        if (mSurfaceHolder != null) {
            mSurfaceHolder.setKeepScreenOn(mScreenOnWhilePlaying && mStayAwake);
        }
    }

    /**
     * Returns the width of the video.
     *
     * @return the width of the video, or 0 if there is no video,
     * no display surface was set, or the width has not been determined
     * yet. The OnVideoSizeChangedListener can be registered via
     * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
     * to provide a notification when the width is available.
     */
    public native int getVideoWidth();

    /**
     * Returns the height of the video.
     *
     * @return the height of the video, or 0 if there is no video,
     * no display surface was set, or the height has not been determined
     * yet. The OnVideoSizeChangedListener can be registered via
     * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
     * to provide a notification when the height is available.
     */
    public native int getVideoHeight();

    /**
     * Checks whether the MediaPlayer is playing.
     *
     * @return true if currently playing, false otherwise
     * @throws IllegalStateException if the internal player engine has not been
     * initialized or has been released.
     */
    public native boolean isPlaying();

    /**
     * Change playback speed of audio by resampling the audio.
     * <p>
     * Specifies resampling as audio mode for variable rate playback, i.e.,
     * resample the waveform based on the requested playback rate to get
     * a new waveform, and play back the new waveform at the original sampling
     * frequency.
     * When rate is larger than 1.0, pitch becomes higher.
     * When rate is smaller than 1.0, pitch becomes lower.
     *
     * @hide
     */
    public static final int PLAYBACK_RATE_AUDIO_MODE_RESAMPLE = 2;

    /**
     * Change playback speed of audio without changing its pitch.
     * <p>
     * Specifies time stretching as audio mode for variable rate playback.
     * Time stretching changes the duration of the audio samples without
     * affecting its pitch.
     * <p>
     * This mode is only supported for a limited range of playback speed factors,
     * e.g. between 1/2x and 2x.
     *
     * @hide
     */
    public static final int PLAYBACK_RATE_AUDIO_MODE_STRETCH = 1;

    /**
     * Change playback speed of audio without changing its pitch, and
     * possibly mute audio if time stretching is not supported for the playback
     * speed.
     * <p>
     * Try to keep audio pitch when changing the playback rate, but allow the
     * system to determine how to change audio playback if the rate is out
     * of range.
     *
     * @hide
     */
    public static final int PLAYBACK_RATE_AUDIO_MODE_DEFAULT = 0;

    /** @hide */
    @IntDef(
        value = {
            PLAYBACK_RATE_AUDIO_MODE_DEFAULT,
            PLAYBACK_RATE_AUDIO_MODE_STRETCH,
            PLAYBACK_RATE_AUDIO_MODE_RESAMPLE,
    })
    @Retention(RetentionPolicy.SOURCE)
    public @interface PlaybackRateAudioMode {}

    /**
     * Sets playback rate and audio mode.
     *
     * @param rate the ratio between desired playback rate and normal one.
     * @param audioMode audio playback mode. Must be one of the supported
     * audio modes.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     * @throws IllegalArgumentException if audioMode is not supported.
     *
     * @hide
     */
    @NonNull
    public PlaybackParams easyPlaybackParams(float rate, @PlaybackRateAudioMode int audioMode) {
        PlaybackParams params = new PlaybackParams();
        params.allowDefaults();
        switch (audioMode) {
        case PLAYBACK_RATE_AUDIO_MODE_DEFAULT:
            params.setSpeed(rate).setPitch(1.0f);
            break;
        case PLAYBACK_RATE_AUDIO_MODE_STRETCH:
            params.setSpeed(rate).setPitch(1.0f)
                    .setAudioFallbackMode(params.AUDIO_FALLBACK_MODE_FAIL);
            break;
        case PLAYBACK_RATE_AUDIO_MODE_RESAMPLE:
            params.setSpeed(rate).setPitch(rate);
            break;
        default:
            final String msg = "Audio playback mode " + audioMode + " is not supported";
            throw new IllegalArgumentException(msg);
        }
        return params;
    }

    /**
     * Sets playback rate using {@link PlaybackParams}. The object sets its internal
     * PlaybackParams to the input, except that the object remembers previous speed
     * when input speed is zero. This allows the object to resume at previous speed
     * when start() is called. Calling it before the object is prepared does not change
     * the object state. After the object is prepared, calling it with zero speed is
     * equivalent to calling pause(). After the object is prepared, calling it with
     * non-zero speed is equivalent to calling start().
     *
     * @param params the playback params.
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized or has been released.
     * @throws IllegalArgumentException if params is not supported.
     */
    public native void setPlaybackParams(@NonNull PlaybackParams params);

    /**
     * Gets the playback params, containing the current playback rate.
     *
     * @return the playback params.
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    @NonNull
    public native PlaybackParams getPlaybackParams();

    /**
     * Sets A/V sync mode.
     *
     * @param params the A/V sync params to apply
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     * @throws IllegalArgumentException if params are not supported.
     */
    public native void setSyncParams(@NonNull SyncParams params);

    /**
     * Gets the A/V sync mode.
     *
     * @return the A/V sync params
     *
     * @throws IllegalStateException if the internal player engine has not been
     * initialized.
     */
    @NonNull
    public native SyncParams getSyncParams();

    /**
     * Seeks to specified time position.
     *
     * @param msec the offset in milliseconds from the start to seek to
     * @throws IllegalStateException if the internal player engine has not been
     * initialized
     */
    public native void seekTo(int msec) throws IllegalStateException;

    /**
     * Get current playback position as a {@link MediaTimestamp}.
     * <p>
     * The MediaTimestamp represents how the media time correlates to the system time in
     * a linear fashion using an anchor and a clock rate. During regular playback, the media
     * time moves fairly constantly (though the anchor frame may be rebased to a current
     * system time, the linear correlation stays steady). Therefore, this method does not
     * need to be called often.
     * <p>
     * To help users get current playback position, this method always anchors the timestamp
     * to the current {@link System#nanoTime system time}, so
     * {@link MediaTimestamp#getAnchorMediaTimeUs} can be used as current playback position.
     *
     * @return a MediaTimestamp object if a timestamp is available, or {@code null} if no timestamp
     *         is available, e.g. because the media player has not been initialized.
     *
     * @see MediaTimestamp
     */
    @Nullable
    public MediaTimestamp getTimestamp()
    {
        try {
            // TODO: get the timestamp from native side
            return new MediaTimestamp(
                    getCurrentPosition() * 1000L,
                    System.nanoTime(),
                    isPlaying() ? getPlaybackParams().getSpeed() : 0.f);
        } catch (IllegalStateException e) {
            return null;
        }
    }

    /**
     * Gets the current playback position.
     *
     * @return the current position in milliseconds
     */
    public native int getCurrentPosition();

    /**
     * Gets the duration of the file.
     *
     * @return the duration in milliseconds, if no duration is available
     *         (for example, if streaming live content), -1 is returned.
     */
    public native int getDuration();

    /**
     * Gets the media metadata.
     *
     * @param update_only controls whether the full set of available
     * metadata is returned or just the set that changed since the
     * last call. See {@see #METADATA_UPDATE_ONLY} and {@see
     * #METADATA_ALL}.
     *
     * @param apply_filter if true only metadata that matches the
     * filter is returned. See {@see #APPLY_METADATA_FILTER} and {@see
     * #BYPASS_METADATA_FILTER}.
     *
     * @return The metadata, possibly empty. null if an error occured.
     // FIXME: unhide.
     * {@hide}
     */
    public Metadata getMetadata(final boolean update_only,
                                final boolean apply_filter) {
        Parcel reply = Parcel.obtain();
        Metadata data = new Metadata();

        if (!native_getMetadata(update_only, apply_filter, reply)) {
            reply.recycle();
            return null;
        }

        // Metadata takes over the parcel, don't recycle it unless
        // there is an error.
        if (!data.parse(reply)) {
            reply.recycle();
            return null;
        }
        return data;
    }

    /**
     * Set a filter for the metadata update notification and update
     * retrieval. The caller provides 2 set of metadata keys, allowed
     * and blocked. The blocked set always takes precedence over the
     * allowed one.
     * Metadata.MATCH_ALL and Metadata.MATCH_NONE are 2 sets available as
     * shorthands to allow/block all or no metadata.
     *
     * By default, there is no filter set.
     *
     * @param allow Is the set of metadata the client is interested
     *              in receiving new notifications for.
     * @param block Is the set of metadata the client is not interested
     *              in receiving new notifications for.
     * @return The call status code.
     *
     // FIXME: unhide.
     * {@hide}
     */
    public int setMetadataFilter(Set<Integer> allow, Set<Integer> block) {
        // Do our serialization manually instead of calling
        // Parcel.writeArray since the sets are made of the same type
        // we avoid paying the price of calling writeValue (used by
        // writeArray) which burns an extra int per element to encode
        // the type.
        Parcel request =  newRequest();

        // The parcel starts already with an interface token. There
        // are 2 filters. Each one starts with a 4bytes number to
        // store the len followed by a number of int (4 bytes as well)
        // representing the metadata type.
        int capacity = request.dataSize() + 4 * (1 + allow.size() + 1 + block.size());

        if (request.dataCapacity() < capacity) {
            request.setDataCapacity(capacity);
        }

        request.writeInt(allow.size());
        for(Integer t: allow) {
            request.writeInt(t);
        }
        request.writeInt(block.size());
        for(Integer t: block) {
            request.writeInt(t);
        }
        return native_setMetadataFilter(request);
    }

    /**
     * Set the MediaPlayer to start when this MediaPlayer finishes playback
     * (i.e. reaches the end of the stream).
     * The media framework will attempt to transition from this player to
     * the next as seamlessly as possible. The next player can be set at
     * any time before completion, but shall be after setDataSource has been
     * called successfully. The next player must be prepared by the
     * app, and the application should not call start() on it.
     * The next MediaPlayer must be different from 'this'. An exception
     * will be thrown if next == this.
     * The application may call setNextMediaPlayer(null) to indicate no
     * next player should be started at the end of playback.
     * If the current player is looping, it will keep looping and the next
     * player will not be started.
     *
     * @param next the player to start after this one completes playback.
     *
     */
    public native void setNextMediaPlayer(MediaPlayer next);

///////////// added by RTK  ////////////////////////////////////

    /**
     * Set the MediaPlayer to use RTMediaPlayer as its player engine
     *
     * This function must be called before calling setDataSource,
     * otherwise, it can't take effect.
     *
     * The funcion only work with the system property
     * "persist.media.USE_RTMediaPlayer" is set to "true" or "1".
     */
    public native void useRTMediaPlayer(int forceType);

    /**
     * set forward speed
     *
     * @return false : fail, true : ok
     */
    public native boolean fastforward(int speed);

    /**
     * set rewind speed
     *
     * @return false : fail, true : ok
     */
    public native boolean fastrewind(int speed);

    /* @Deprecated 20140313
     * getSubtitleInfo
     * @param streamNum : 1-based. -1 means current subtitle stream.
     * @return long array:
     *    0 : responseSize;
     *    1 : numStreams - total number of spu streams. 0 means no spu.
     *    2 : currentStreamNum - current playback spu stream number. (1-based) , -1 means spu stream is not set yet.
     *    3 : langCode  - ISO-639 language code
     *    4 : spuFrom - 
     *           1 : external spu stream                  
     *           2 : embedded spu stream
     *           3 : embedded ts stream
     *           4 : embedded ts tt stream
     */
    @Deprecated
    public native int[] getSubtitleInfo(int streamNum);
    @Deprecated
    public int[] getSubtitleInfo()
    {
        return getSubtitleInfo(-1);
    }

   /*
     * setAutoScanExtSubtitle : must set before prepare or prepareAsync
     *
     * @param autoScan : 
     *     1 : auto scan external subtitle. (default)
     *     0 : manual set subtitle path
     * @return void
     */
    public native void setAutoScanExtSubtitle(int autoScan);

   /*
     * setExtSubtitlePath
     *
     * @param path : full subtitle file path
     * @return void
     */
    public native void setExtSubtitlePath(String path);

    /*
     * setSubtitleInfo
     *
     * @param streamNum - spu stream number (1-based). -1 means current playing stream
     * @param enable - 1 means turn on spu while 0 means turn off
     * @param textEncoding - character encoding. For encoding map, read TEXT_ENCODING_TABLE.
     * @param textColor - color of character. For color map, read TEXT_COLOR_TABLE.
     * @param fontSize - size of character. min is 12 while max is 76.
     *         TEXT_ENCODING_TABLE - 
     *              0 : UTF8
     *              1 : GBK
     *              2 : BIG5
     *              3 : WESTERN
     *              4 : TURKISH
     *              5 : CENTRAL EUROPEAN (CP 1250)
     *              6 : GREEK (CP 1253)
     *              7 : CYRILLIC (CP 1251)
     *              8 : HEBREW   
     *              9 : SOUTH EASTERN EUROPEAN
     *             10 : JAPANESE
     *             11 : KOREAN
     *             12 : THAI  
     *             13 : ARABIC
     *             14 : CENTRAL EUROPEAN (ISO 8859-2)
     *             15 : GREEK (ISO 8859-7)
     *             16 : CYRILLIC (KO18-R)
     *           1000 : AUTO DETECT
     *             
     *         TEXT_COLOR_TABLE - 
     *              0 : WHITE
     *              1 : BLACK
     *              2 : RED
     *              3 : GREEN
     *              4 : BLUE
     *              5 : YELLOW
     * @return void
     */
    public native void setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize, int syncTime, int offset);

    /*
     * getAudioTrackInfo
     * @param streamNum : 1-based. -1 means current audio stream
     * @return int array:
     *    0 : responseSize;
     *    1 : numStreams - total number of audio streams. 0 means no audio.
     *    2 : currentStreamNum - current playback audio stream number. (1-based) , -1 means no audio is playing now.
     *    3 : type - audio type string.
     *    4 : chNum - channel number
     *    5 : bitsPerSample -
     *    6 : sampleRate - samples per second.
     *    7 : pid - 0x1FFF means unknown
     */
    public native int[] getAudioTrackInfo(int streamNum);

    /* @Deprecated 20140313
     * setAudioTrackInfo
     *
     * @param streamNum : 1-based. Use getAudioTrackInfo to get total number of audio streams.
     * @return void
     */
    @Deprecated
    public native void setAudioTrackInfo(int streamNum);

    public static final int MEDIA_PLAYER_STATE_ERROR = 0;
    public static final int MEDIA_PLAYER_IDLE = 1;
    public static final int MEDIA_PLAYER_INITIALIZED = 2;
    public static final int MEDIA_PLAYER_PREPARING = 4;
    public static final int MEDIA_PLAYER_PREPARED = 8;
    public static final int MEDIA_PLAYER_STARTED = 16;
    public static final int MEDIA_PLAYER_PAUSED = 32;
    public static final int MEDIA_PLAYER_STOPPED = 64;
    public static final int MEDIA_PLAYER_PLAYBACK_COMPLETE = 128;
    /**
     * Gets the state of the file. The original definition as following list.
     *     MEDIA_PLAYER_STATE_ERROR        = 0,
     *     MEDIA_PLAYER_IDLE               = 1 << 0,
     *     MEDIA_PLAYER_INITIALIZED        = 1 << 1,
     *     MEDIA_PLAYER_PREPARING          = 1 << 2,
     *     MEDIA_PLAYER_PREPARED           = 1 << 3,
     *     MEDIA_PLAYER_STARTED            = 1 << 4,
     *     MEDIA_PLAYER_PAUSED             = 1 << 5,
     *     MEDIA_PLAYER_STOPPED            = 1 << 6,
     *     MEDIA_PLAYER_PLAYBACK_COMPLETE  = 1 << 7
     *
     * @return the state in interger
     */
    public native int getPlayerCurrentState();

    /**
      * Please reference following id as propertyID .
    static struct{int id; NAV_PROP_ID navID; } NAV_PROPERTY_ID_MAP[] = 
    {
        {0, NAVPROP_DIVX_DRM_QUERY},
        {1, NAVPROP_DIVX_DRM_APPROVE_RENTAL},
        {2, NAVPROP_DIVX_EDITIONNUM_QUERY},
        {3, NAVPROP_DIVX_EDITIONNAME_QUERY},
        {4, NAVPROP_DIVX_EDITIONLAWRATE_QUERY},
        {5, NAVPROP_DIVX_METADATA_QUERY},
        {6, NAVPROP_DIVX_LAWRATE_QUERY},
        {7, NAVPROP_DIVX_TITLENUM_QUERY},
        {8, NAVPROP_DIVX_CHAPTERTYPE_QUERY},
        {9, NAVPROP_DIVX_LASTPLAYPATH},
    };
    */
    public static final int NAVPROP_DIVX_DRM_QUERY = 0;
    public static final int NAVPROP_DIVX_DRM_APPROVE_RENTAL = 1;
    public static final int NAVPROP_DIVX_EDITIONNUM_QUERY = 2;
    public static final int NAVPROP_DIVX_EDITIONNAME_QUERY = 3;
    public static final int NAVPROP_DIVX_EDITIONLAWRATE_QUERY = 4;
    public static final int NAVPROP_DIVX_METADATA_QUERY = 5;
    public static final int NAVPROP_DIVX_LAWRATE_QUERY = 6;
    public static final int NAVPROP_DIVX_TITLENUM_QUERY = 7;
    public static final int NAVPROP_DIVX_CHAPTERTYPE_QUERY = 8;
    public static final int NAVPROP_DIVX_LASTPLAYPATH = 9;
    public native byte[] execSetGetNavProperty(int propertyID,byte[] inputArray);

    /* Do not change these values (starting with NAVI_DOMAIN_) without updating
     * their counterparts in cmdDvdPlayer\system\src\Include\Filters\NavigationFilter\NavDef.h
     */
    public static final int NAVI_DOMAIN_STOP = 0;
    public static final int NAVI_DOMAIN_TITLE = 1;
    public static final int NAVI_DOMAIN_MENU = 2;
    public static final int NAVI_DOMAIN_VR_PG = 3;
    public static final int NAVI_DOMAIN_VR_PL = 4;
    
    /* Do not change these values (starting with NAVI_) without updating
     * their counterparts in device/realtek/proprietary/libs/libRTMediaPlayer/RTMediaPlayer.h
     */
    public static final int NAVI_UP = 0;
    public static final int NAVI_DOWN = 1;
    public static final int NAVI_LEFT = 2;
    public static final int NAVI_RIGHT = 3;
    public static final int NAVI_ENTER = 4;
    public static final int NAVI_CHANGE_TITLE = 5;
    public static final int NAVI_CHANGE_CHAPTER = 6;
    public static final int NAVI_CHANGE_ANGLE = 7;
    public static final int NAVI_SWITCH_CHANNEL = 8; // for ts
    public static final int NAVI_DVD_ROOT_MENU = 9;
    public static final int NAVI_DVD_TITLE_MENU = 10;
    public static final int NAVI_BDHDMV_HOME_MENU = 11;
    public static final int NAVI_BDHDMV_POPUP_MENU = 12;
    /**
     * Navigation control if player supports the function.
     *
     * @param action refer to the above NAVI_*
     * @param target if action is one of the ranage {NAVI_UP, NAVI_ENTER}, it is donot-care.
     *               if action is NAVI_CHANGE_*, it is the target number.
     *               if action is NAVI_SWITCH_CHANNEL, 0 indicates switching to previous channel
     *                                                 1 indicates switching to next channel
     *
     * @return true if Nav command is successfully processed; false otherwise
     */
    public native boolean deliverNaviControlCmd(int action, int target);

    /*
     * setStartTime
     *
     * @param msec : starting time (ms) for the playback
     * @return void
     */
    public native void setStartTime(int msec);

    private void stopMeasureThread() {
        synchronized(this)
        {
            if (null != measure) {
                Log.d(TAG, "interrupting SpeedMeasureThread...");
                measure.interrupt();
                measure = null;
            }
        }
    }

    /**
     * setTvSystem - It is used to set TV system info to RTMediaPlayer to have correct
     *               video output before starting playback. After SetDataSource, this
     *               method should be called to configure RTMediaPlayer.
     *
     * @param width - the TV resolution width.
     * @param height - the TV resolution height
     * @param framerate -  the TV display framerate ( 50 or 60 )
     * @param isProgressive - progressive or interfaced.
     *
     * @return void
     */
    public void setTvSystem(int width, int height, int framerate, boolean isProgressive) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_TV_SYSTEM_TYPE);
            request.writeInt(width);
            request.writeInt(height);
            request.writeInt(framerate);
            request.writeInt(isProgressive ? 1 : 0);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * setLanguages - It is used to set the default settings before starting playback. After SetDataSource,
     *                this method should be called to configure RTMediaPlayer.
     *
     * @param menu_lang - menu language. Set RTMediaPlayer which language to be used. Refer to MENU_LANGUAGE_TABLE
     * @param sub_lang - subtitle language. Set RTMediaPlayer which subtitle language to be selected
     * @param sub_text_encoding - subtitle text encoding. For encoding map, refer to TEXT_ENCODING_TABLE
     * @param audio_lang - audio language. Set RTMediaPlayer which audio language stream to be selected.
     *
     * @deprecated This method is not necessary for RTMediaPlayer because it is not in charge of OSD and MENU.
     * @return void
     */
    public void setLanguages(int osd_lang, int menu_lang, int sub_lang, int sub_text_encoding, int audio_lang) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_LANGUAGES);
            request.writeInt(osd_lang);
            request.writeInt(menu_lang);
            request.writeInt(sub_lang);
            request.writeInt(sub_text_encoding);
            request.writeInt(audio_lang);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /* Do not change these values (starting with NAVPROP_) without updating
     * their counterparts in device/realtek/proprietary/libs/libRTMediaPlayer/RTMediaPlayer.h
     */
    private static final int NAVPROP_SET_STEREOSCOPIC_OUTPUT_MODE = 1;
    private static final int NAVPROP_GET_PLAYBACK_STATUS = 2;
    private static final int NAVPROP_GET_MEDIA_TYPE = 3;
    private static final int NAVPROP_SET_STREAMING_IO_BUFFER_THRESHOLD = 4;
    private static final int NAVPROP_GET_BD_ISO_INFO = 5;
    private static final int NAVPROP_GET_BD_ISO_CHAPTER_INFO = 6;

    /**
     * setMVC - It is used to enable or disable MVC (Multiview Video Coding) (3D video).
     *
     * @param bEnabled - true means enable MVC; otherwise disable MVC mode.
     * @return void
     */
    public void setMVC(boolean bEnabled) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_SET_STEREOSCOPIC_OUTPUT_MODE);
            request.writeInt(bEnabled?1:0);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Class for MediaPlayer to return navigation info including
     * number of titles, number of chapters and number of angels.
     *
     * @see android.media.MediaPlayer#getNaviInfo
     */
    static public class NaviInfo implements Parcelable {
        /**
         * Get the {@link NavigationRecord} of the navigation. If the record is
         * unknown or could not be determined, null is returned.
         */
        public NavigationRecord getRecord() {
            return mRecord;
        }

        final NavigationRecord mRecord;

        NaviInfo(Parcel in) {
            int numOfField = in.readInt();
            int domain = in.readInt();
            int numOfTitles = in.readInt();
            int numOfChapters = in.readInt();
            int numOfAngles = in.readInt();
            mRecord = NavigationRecord.createNavigationRecord(
                        domain, numOfTitles, numOfChapters, numOfAngles);
            if (numOfField == 7) {
                mRecord.setInteger(NavigationRecord.KEY_CURRENT_TITLE, in.readInt());
                mRecord.setInteger(NavigationRecord.KEY_CURRENT_CHAPTER, in.readInt());
                mRecord.setInteger(NavigationRecord.KEY_CURRENT_ANGLE, in.readInt());
            }
            Log.d(TAG, "got navigation record: " + mRecord.toString());
        }

        /** @hide */
        NaviInfo(NavigationRecord record) {
            mRecord = record;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public int describeContents() {
            return 0;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void writeToParcel(Parcel dest, int flags) {
            int numOfFields = mRecord.size();
            dest.writeInt(numOfFields);
            dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_DOMAIN));
            dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_NUM_OF_TITLES));
            dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_NUM_OF_CHAPTERS));
            dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_NUM_OF_ANGLES));
            if (numOfFields == 7) {
                dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_CURRENT_TITLE));
                dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_CURRENT_CHAPTER));
                dest.writeInt(mRecord.getInteger(NavigationRecord.KEY_CURRENT_ANGLE));
            }
        }

        /**
         * Used to read a NaviInfo from a Parcel
         */
        static final Parcelable.Creator<NaviInfo> CREATOR
                = new Parcelable.Creator<NaviInfo>() {
                    @Override
                    public NaviInfo createFromParcel(Parcel in) {
                        return new NaviInfo(in);
                    }

                    @Override
                    public NaviInfo[] newArray(int size) {
                        return new NaviInfo[size];
                    }
                };
    };

    public NaviInfo getNavigationInfo() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_GET_PLAYBACK_STATUS);
            invoke(request, reply);
            NaviInfo naviInfo = NaviInfo.CREATOR.createFromParcel(reply);
            return naviInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Got exception: ", e);
            return null;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Class for MediaPlayer to return navigation info including
     * lists of titles.
     *
     * @see android.media.MediaPlayer#getBDInfo
     */
    static public class BDInfo implements Parcelable {

        public ArrayList<Integer> getInfo() {
            return mList;
        }

        private ArrayList<Integer> mList = new ArrayList<Integer>();

        BDInfo(Parcel in) {
            int numOfField = in.readInt();
            int numOfTitles = in.readInt();
            mList.add(numOfTitles);

            for(int index=0;index<numOfTitles;index++)
            {
                mList.add(in.readInt());
            }
//            Log.d(TAG, "got BD info(): " + mList.toString());
        }

        public int getSize() {
            return mList.get(0);
        }

        public int getDuration(int position) {
            return mList.get(position);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public int describeContents() {
            return 0;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void writeToParcel(Parcel dest, int flags) {
            int numOfFields = mList.size()+1;
            dest.writeInt(numOfFields);
            for(int index=0;index<mList.size();index++)
            {
                dest.writeInt(mList.get(index));
            }
        }

        /**
         * Used to read a BDInfo from a Parcel
         */
        static final Parcelable.Creator<BDInfo> CREATOR
                = new Parcelable.Creator<BDInfo>() {
                    @Override
                    public BDInfo createFromParcel(Parcel in) {
                        return new BDInfo(in);
                    }

                    @Override
                    public BDInfo[] newArray(int size) {
                        return new BDInfo[size];
                    }
                };
    };

    public BDInfo getBDInfo() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_GET_BD_ISO_INFO);
            invoke(request, reply);
            BDInfo bdInfo = BDInfo.CREATOR.createFromParcel(reply);
            return bdInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Got exception: ", e);
            return null;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    public BDInfo getBDChapterInfo() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_GET_BD_ISO_CHAPTER_INFO);
            invoke(request, reply);
            BDInfo bdInfo = BDInfo.CREATOR.createFromParcel(reply);
            return bdInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Got exception: ", e);
            return null;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Class for MediaPlayer to return navigation media type.
     *
     * @see android.media.MediaPlayer#getNaviType
     */
    static public class NaviType implements Parcelable {
        /** @hide */
        public MediaType getType() {
            return mType;
        }

        final MediaType mType;

        NaviType(Parcel in) {
            int numOfField = in.readInt();
            int majorType = in.readInt();
            int subType = in.readInt();
            mType = MediaType.createMediaType(majorType, subType);
            Log.d(TAG, "got navigation type: " + mType.toString());
        }

        /** @hide */
        NaviType(MediaType type) {
            mType = type;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public int describeContents() {
            return 0;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void writeToParcel(Parcel dest, int flags) {
        }

        /**
         * Used to read a NaviType from a Parcel
         */
        static final Parcelable.Creator<NaviType> CREATOR
                = new Parcelable.Creator<NaviType>() {
                    @Override
                    public NaviType createFromParcel(Parcel in) {
                        return new NaviType(in);
                    }

                    @Override
                    public NaviType[] newArray(int size) {
                        return new NaviType[size];
                    }
                };
    };

    public NaviType getNavigationType() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_GET_MEDIA_TYPE);
            invoke(request, reply);
            NaviType naviType = NaviType.CREATOR.createFromParcel(reply);
            return naviType;
        } catch (RuntimeException e) {
            Log.w(TAG, "Got exception: ", e);
            return null;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Set the streaming buffer threshold to playback engine.
     *
     * @param highThreshold high water threshold in bytes
     * @param lowThreshold  low water threshold in bytes
     */
    public void setStreamingBufferThreshold(int highThreshold, int lowThreshold) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_GET_NAV_PROPERTY);
            request.writeInt(NAVPROP_SET_STREAMING_IO_BUFFER_THRESHOLD);
            request.writeInt(highThreshold);
            request.writeInt(lowThreshold);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /* Do not change these values (starting with INVOKE_ID) without updating
     * their counterparts in RTMediaPlayer.h!
     */
    public static final int BOOKMARK_OP_GET = 1;
    public static final int BOOKMARK_OP_SET = 2;
    public static final int BOOKMARK_OP_REMOVE = 3;
    public static final int BOOKMARK_OP_APPLY = 4;
    /**
     * setBookmark - It is used to set a bookmark of the input content URI.
     *
     * @param uri URI of video content.
     * @return void
     */
    public boolean setBookmark(Uri uri) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        String contentPath = Uri.decode(uri.toString());
        int ret = 0;
        try {
            Log.d(TAG, "setBookmark, contentPath: " + contentPath);
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_BOOKMARK_OPERATION);
            request.writeInt(BOOKMARK_OP_SET);
            request.writeString(contentPath);
            invoke(request, reply);

            ret = reply.readInt();
            Log.d(TAG, "setBookmark, contentPath: " + contentPath + " ret: " + ret);

            return (ret > 0) ? true:false;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * applyBookmark - It is used to apply a previously saved bookmark of the input content URI.
     *
     * @param uri URI of video content.
     * @return void
     */
    public boolean applyBookmark(Uri uri) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        String contentPath = Uri.decode(uri.toString());
        int ret = 0;
        try {
            Log.d(TAG, "applyBookmark, contentPath: " + contentPath);
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_BOOKMARK_OPERATION);
            request.writeInt(BOOKMARK_OP_APPLY);
            request.writeString(contentPath);
            invoke(request, reply);

            ret = reply.readInt();
            Log.d(TAG, "applyBookmark, contentPath: " + contentPath + " ret: " + ret);

            return (ret > 0) ? true:false;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * checkBookmark - Check if there is a previously saved bookmark of the input content URI.
     *
     * @param uri URI of video content.
     * @return True if there is a previously saved bookmark of the input content URI.
     */
    public boolean checkBookmark(Uri uri) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        String contentPath = Uri.decode(uri.toString());
        int ret = 0;
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_BOOKMARK_OPERATION);
            request.writeInt(BOOKMARK_OP_GET);
            request.writeString(contentPath);
            invoke(request, reply);
            
            ret = reply.readInt();
            Log.d(TAG, "checkBookmark, contentPath: " + contentPath + " ret: " + ret);

            return (ret > 0) ? true:false;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }
    /**
     * removeBookmark - It is used to remove a previously saved bookmark of the input content URI.
     *
     * @param uri URI of video content.
     * @return void
     */
    public boolean removeBookmark(Uri uri) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        String contentPath = Uri.decode(uri.toString());
        int ret = 0;
        try {
            Log.d(TAG, "removeBookmark, contentPath: " + contentPath);
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_BOOKMARK_OPERATION);
            request.writeInt(BOOKMARK_OP_REMOVE);
            request.writeString(contentPath);
            invoke(request, reply);

            ret = reply.readInt();
            Log.d(TAG, "removeBookmark, contentPath: " + contentPath + " ret: " + ret);

            return (ret > 0) ? true:false;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /* Do not change these values (starting with INVOKE_ID) without updating
     * their counterparts in RTMediaPlayer.h!
     */
    public static final int AUDIO_OP_GET_TRACK = 1;
    public static final int AUDIO_OP_SET_TRACK = 2;
    public static final int AUDIO_OP_GET_CURRENT_INDEX = 3;
    /*
    * getSubtitleTrack: get all the subtitle tracks
    */
    public TrackInfo[] getAudioTrackInfo(boolean bForceUpdate) throws IllegalStateException {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            Log.v(TAG, "getAudioTrackInfo");
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_AUDIO_OPERATION);
            request.writeInt(AUDIO_OP_GET_TRACK);
            request.writeInt(bForceUpdate? 1:0);
            invoke(request, reply);

            TrackInfo trackInfo[] = reply.createTypedArray(TrackInfo.CREATOR);
            return trackInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Illegal state to get track info: " + e);
            TrackInfo trackInfo[] = new TrackInfo[0];
            return trackInfo;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /*
    * getAudioCurrentIndex: get the current audio track index (1-based)
    *
    * @parameter: None
    * @return: integer
    */
    public int getAudioCurrentIndex() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        int ret = 0;

        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_AUDIO_OPERATION);
            request.writeInt(AUDIO_OP_GET_CURRENT_INDEX);
            invoke(request, reply);
            ret = reply.readInt();
            Log.d(TAG, "getAudioCurrentIndex, ret: " + ret);

            return ret;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }


    /* Do not change these values (starting with INVOKE_ID) without updating
     * their counterparts in RTMediaPlayer.h!
     */
    public static final int SUBTITLE_OP_GET_TRACK = 1;
    public static final int SUBTITLE_OP_SET_TRACK = 2;
    public static final int SUBTITLE_OP_GET_CURRENT_INDEX = 3;

    /*
    * getSubtitleTrack: get all the subtitle tracks
    */
    public TrackInfo[] getSubtitleTrackInfo(boolean bForceUpdate) throws IllegalStateException {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            Log.v(TAG, "getSubtitleTrackInfo");
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_SUBTITLE_OPERATION);
            request.writeInt(SUBTITLE_OP_GET_TRACK);
            request.writeInt(bForceUpdate? 1:0);
            invoke(request, reply);

            TrackInfo trackInfo[] = reply.createTypedArray(TrackInfo.CREATOR);
            return trackInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Illegal state to get track info: " + e);
            TrackInfo trackInfo[] = new TrackInfo[0];
            return trackInfo;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /*
    * getSubtitleCurrentIndex: get the current subtitle track index (1-based)
    *
    * @parameter: None
    * @return: integer
    */

    public int getSubtitleCurrentIndex() {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        int ret = 0;

        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_EXEC_SUBTITLE_OPERATION);
            request.writeInt(SUBTITLE_OP_GET_CURRENT_INDEX);
            invoke(request, reply);
            ret = reply.readInt();
            Log.d(TAG, "getSubtitleCurrentIndex, ret: " + ret);

            return ret;
        } finally {
            request.recycle();
            reply.recycle();
        }        
    }

    /**
     * setVideoScaledSize - It is used to set video scaled size to RTMediaPlayer to 
     * scale video by vpu
     * @param width - the scaled width.
     * @param height - the scaled height.
     * @return void
     */
    public void setVideoScaledSize(int width, int height) {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_SET_VIDEO_SCALED_SIZE);
            request.writeInt(width);
            request.writeInt(height);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }    
///////////// end of RTK //////////////////////////////////////////////////

    /**
     * Releases resources associated with this MediaPlayer object.
     * It is considered good practice to call this method when you're
     * done using the MediaPlayer. In particular, whenever an Activity
     * of an application is paused (its onPause() method is called),
     * or stopped (its onStop() method is called), this method should be
     * invoked to release the MediaPlayer object, unless the application
     * has a special need to keep the object around. In addition to
     * unnecessary resources (such as memory and instances of codecs)
     * being held, failure to call this method immediately if a
     * MediaPlayer object is no longer needed may also lead to
     * continuous battery consumption for mobile devices, and playback
     * failure for other applications if no multiple instances of the
     * same codec are supported on a device. Even if multiple instances
     * of the same codec are supported, some performance degradation
     * may be expected when unnecessary multiple instances are used
     * at the same time.
     */
    public void release() {
        baseRelease();
        stayAwake(false);
        updateSurfaceScreenOn();
        //[RTK Begin]
        stopMeasureThread();
        //[RTK End];
        mOnPreparedListener = null;
        mOnBufferingUpdateListener = null;
        mOnCompletionListener = null;
        mOnSeekCompleteListener = null;
        mOnSeekResultListener = null;
        mOnErrorListener = null;
        mOnInfoListener = null;
        mOnVideoSizeChangedListener = null;
        mOnTimedTextListener = null;
        if (mTimeProvider != null) {
            mTimeProvider.close();
            mTimeProvider = null;
        }
        mOnSubtitleDataListener = null;
        mOnSpeedChangedListener = null;
        _release();
    }

    private native void _release();

    /**
     * Resets the MediaPlayer to its uninitialized state. After calling
     * this method, you will have to initialize it again by setting the
     * data source and calling prepare().
     */
    public void reset() {
        mSelectedSubtitleTrackIndex = -1;
        synchronized(mOpenSubtitleSources) {
            for (final InputStream is: mOpenSubtitleSources) {
                try {
                    is.close();
                } catch (IOException e) {
                }
            }
            mOpenSubtitleSources.clear();
        }
        if (mSubtitleController != null) {
            mSubtitleController.reset();
        }
        if (mTimeProvider != null) {
            mTimeProvider.close();
            mTimeProvider = null;
        }

        stayAwake(false);
        _reset();
        // make sure none of the listeners get called anymore
        if (mEventHandler != null) {
            mEventHandler.removeCallbacksAndMessages(null);
        }

        synchronized (mIndexTrackPairs) {
            mIndexTrackPairs.clear();
            mInbandTrackIndices.clear();
        };
    }

    private native void _reset();

    /**
     * Sets the audio stream type for this MediaPlayer. See {@link AudioManager}
     * for a list of stream types. Must call this method before prepare() or
     * prepareAsync() in order for the target stream type to become effective
     * thereafter.
     *
     * @param streamtype the audio stream type
     * @see android.media.AudioManager
     */
    public void setAudioStreamType(int streamtype) {
        baseUpdateAudioAttributes(
                new AudioAttributes.Builder().setInternalLegacyStreamType(streamtype).build());
        _setAudioStreamType(streamtype);
        mStreamType = streamtype;
    }

    private native void _setAudioStreamType(int streamtype);

    // Keep KEY_PARAMETER_* in sync with include/media/mediaplayer.h
    private final static int KEY_PARAMETER_AUDIO_ATTRIBUTES = 1400;
    /**
     * Sets the parameter indicated by key.
     * @param key key indicates the parameter to be set.
     * @param value value of the parameter to be set.
     * @return true if the parameter is set successfully, false otherwise
     * {@hide}
     */
    public native boolean setParameter(int key, Parcel value);

    /**
     * Sets the audio attributes for this MediaPlayer.
     * See {@link AudioAttributes} for how to build and configure an instance of this class.
     * You must call this method before {@link #prepare()} or {@link #prepareAsync()} in order
     * for the audio attributes to become effective thereafter.
     * @param attributes a non-null set of audio attributes
     */
    public void setAudioAttributes(AudioAttributes attributes) throws IllegalArgumentException {
        if (attributes == null) {
            final String msg = "Cannot set AudioAttributes to null";
            throw new IllegalArgumentException(msg);
        }
        baseUpdateAudioAttributes(attributes);
        mUsage = attributes.getUsage();
        mBypassInterruptionPolicy = (attributes.getAllFlags()
                & AudioAttributes.FLAG_BYPASS_INTERRUPTION_POLICY) != 0;
        Parcel pattributes = Parcel.obtain();
        attributes.writeToParcel(pattributes, AudioAttributes.FLATTEN_TAGS);
        setParameter(KEY_PARAMETER_AUDIO_ATTRIBUTES, pattributes);
        pattributes.recycle();
    }

    /**
     * Sets the player to be looping or non-looping.
     *
     * @param looping whether to loop or not
     */
    public native void setLooping(boolean looping);

    /**
     * Checks whether the MediaPlayer is looping or non-looping.
     *
     * @return true if the MediaPlayer is currently looping, false otherwise
     */
    public native boolean isLooping();

    /**
     * Sets the volume on this player.
     * This API is recommended for balancing the output of audio streams
     * within an application. Unless you are writing an application to
     * control user settings, this API should be used in preference to
     * {@link AudioManager#setStreamVolume(int, int, int)} which sets the volume of ALL streams of
     * a particular type. Note that the passed volume values are raw scalars in range 0.0 to 1.0.
     * UI controls should be scaled logarithmically.
     *
     * @param leftVolume left volume scalar
     * @param rightVolume right volume scalar
     */
    /*
     * FIXME: Merge this into javadoc comment above when setVolume(float) is not @hide.
     * The single parameter form below is preferred if the channel volumes don't need
     * to be set independently.
     */
    public void setVolume(float leftVolume, float rightVolume) {
        baseSetVolume(leftVolume, rightVolume);
    }

    @Override
    void playerSetVolume(float leftVolume, float rightVolume) {
        _setVolume(leftVolume, rightVolume);
    }

    private native void _setVolume(float leftVolume, float rightVolume);

    /**
     * Similar, excepts sets volume of all channels to same value.
     * @hide
     */
    public void setVolume(float volume) {
        setVolume(volume, volume);
    }

    /**
     * Sets the audio session ID.
     *
     * @param sessionId the audio session ID.
     * The audio session ID is a system wide unique identifier for the audio stream played by
     * this MediaPlayer instance.
     * The primary use of the audio session ID  is to associate audio effects to a particular
     * instance of MediaPlayer: if an audio session ID is provided when creating an audio effect,
     * this effect will be applied only to the audio content of media players within the same
     * audio session and not to the output mix.
     * When created, a MediaPlayer instance automatically generates its own audio session ID.
     * However, it is possible to force this player to be part of an already existing audio session
     * by calling this method.
     * This method must be called before one of the overloaded <code> setDataSource </code> methods.
     * @throws IllegalStateException if it is called in an invalid state
     */
    public native void setAudioSessionId(int sessionId)  throws IllegalArgumentException, IllegalStateException;

    /**
     * Returns the audio session ID.
     *
     * @return the audio session ID. {@see #setAudioSessionId(int)}
     * Note that the audio session ID is 0 only if a problem occured when the MediaPlayer was contructed.
     */
    public native int getAudioSessionId();

    /**
     * Attaches an auxiliary effect to the player. A typical auxiliary effect is a reverberation
     * effect which can be applied on any sound source that directs a certain amount of its
     * energy to this effect. This amount is defined by setAuxEffectSendLevel().
     * See {@link #setAuxEffectSendLevel(float)}.
     * <p>After creating an auxiliary effect (e.g.
     * {@link android.media.audiofx.EnvironmentalReverb}), retrieve its ID with
     * {@link android.media.audiofx.AudioEffect#getId()} and use it when calling this method
     * to attach the player to the effect.
     * <p>To detach the effect from the player, call this method with a null effect id.
     * <p>This method must be called after one of the overloaded <code> setDataSource </code>
     * methods.
     * @param effectId system wide unique id of the effect to attach
     */
    public native void attachAuxEffect(int effectId);


    /**
     * Sets the send level of the player to the attached auxiliary effect.
     * See {@link #attachAuxEffect(int)}. The level value range is 0 to 1.0.
     * <p>By default the send level is 0, so even if an effect is attached to the player
     * this method must be called for the effect to be applied.
     * <p>Note that the passed level value is a raw scalar. UI controls should be scaled
     * logarithmically: the gain applied by audio framework ranges from -72dB to 0dB,
     * so an appropriate conversion from linear UI input x to level is:
     * x == 0 -> level = 0
     * 0 < x <= R -> level = 10^(72*(x-R)/20/R)
     * @param level send level scalar
     */
    public void setAuxEffectSendLevel(float level) {
        baseSetAuxEffectSendLevel(level);
    }

    @Override
    int playerSetAuxEffectSendLevel(float level) {
        _setAuxEffectSendLevel(level);
        return AudioSystem.SUCCESS;
    }

    private native void _setAuxEffectSendLevel(float level);

    /*
     * @param request Parcel destinated to the media player. The
     *                Interface token must be set to the IMediaPlayer
     *                one to be routed correctly through the system.
     * @param reply[out] Parcel that will contain the reply.
     * @return The status code.
     */
    private native final int native_invoke(Parcel request, Parcel reply);


    /*
     * @param update_only If true fetch only the set of metadata that have
     *                    changed since the last invocation of getMetadata.
     *                    The set is built using the unfiltered
     *                    notifications the native player sent to the
     *                    MediaPlayerService during that period of
     *                    time. If false, all the metadatas are considered.
     * @param apply_filter  If true, once the metadata set has been built based on
     *                     the value update_only, the current filter is applied.
     * @param reply[out] On return contains the serialized
     *                   metadata. Valid only if the call was successful.
     * @return The status code.
     */
    private native final boolean native_getMetadata(boolean update_only,
                                                    boolean apply_filter,
                                                    Parcel reply);

    /*
     * @param request Parcel with the 2 serialized lists of allowed
     *                metadata types followed by the one to be
     *                dropped. Each list starts with an integer
     *                indicating the number of metadata type elements.
     * @return The status code.
     */
    private native final int native_setMetadataFilter(Parcel request);

    private static native final void native_init();
    private native final void native_setup(Object mediaplayer_this);
    private native final void native_finalize();

    /**
     * Class for MediaPlayer to return each audio/video/subtitle track's metadata.
     *
     * @see android.media.MediaPlayer#getTrackInfo
     */
    static public class TrackInfo implements Parcelable {
        /**
         * Gets the track type.
         * @return TrackType which indicates if the track is video, audio, timed text.
         */
        public int getTrackType() {
            return mTrackType;
        }

        /**
         * Gets the language code of the track.
         * @return a language code in either way of ISO-639-1 or ISO-639-2.
         * When the language is unknown or could not be determined,
         * ISO-639-2 language code, "und", is returned.
         */
        public String getLanguage() {
            String language = mFormat.getString(MediaFormat.KEY_LANGUAGE);
            return language == null ? "und" : language;
        }

        /**
         * Gets the {@link MediaFormat} of the track.  If the format is
         * unknown or could not be determined, null is returned.
         */
        public MediaFormat getFormat() {
            if (mTrackType == MEDIA_TRACK_TYPE_TIMEDTEXT
                    || mTrackType == MEDIA_TRACK_TYPE_SUBTITLE
                    || mTrackType == MEDIA_TRACK_TYPE_VIDEO
                    || mTrackType == MEDIA_TRACK_TYPE_AUDIO) {
                return mFormat;
            }
            return null;
        }

        public static final int MEDIA_TRACK_TYPE_UNKNOWN = 0;
        public static final int MEDIA_TRACK_TYPE_VIDEO = 1;
        public static final int MEDIA_TRACK_TYPE_AUDIO = 2;
        public static final int MEDIA_TRACK_TYPE_TIMEDTEXT = 3;
        public static final int MEDIA_TRACK_TYPE_SUBTITLE = 4;
        public static final int MEDIA_TRACK_TYPE_METADATA = 5;

        final int mTrackType;
        final MediaFormat mFormat;

        TrackInfo(Parcel in) {
            // get the field count
            int curPos = in.dataPosition();
            in.setDataPosition(curPos-4);
            int numOfField = in.readInt();

            mTrackType = in.readInt();
            // TODO: parcel in the full MediaFormat; currently we are using createSubtitleFormat
            // even for audio/video tracks, meaning we only set the mime and language.
            String mime = in.readString();
            String language = in.readString();
            //mFormat = MediaFormat.createSubtitleFormat(mime, language);

            if (mTrackType == MEDIA_TRACK_TYPE_SUBTITLE) {
                /*
                mFormat.setInteger(MediaFormat.KEY_IS_AUTOSELECT, in.readInt());
                mFormat.setInteger(MediaFormat.KEY_IS_DEFAULT, in.readInt());
                mFormat.setInteger(MediaFormat.KEY_IS_FORCED_SUBTITLE, in.readInt());
                */
                mFormat = MediaFormat.createSubtitleFormat(mime, language);
                mFormat.setInteger(MediaFormat.KEY_IS_AUTOSELECT, in.readInt());
                mFormat.setInteger(MediaFormat.KEY_IS_DEFAULT, in.readInt());
                mFormat.setInteger(MediaFormat.KEY_IS_FORCED_SUBTITLE, in.readInt());

                Log.d(TAG, "got MEDIA_TRACK_TYPE_SUBTITLE("+numOfField+"): " + mFormat.toString());
            } else if (mTrackType == MEDIA_TRACK_TYPE_TIMEDTEXT) {
                mFormat = MediaFormat.createSubtitleFormat(
                    MEDIA_MIMETYPE_TEXT_SUBRIP, language);
                if (numOfField == 6) {  //RTMediaPlayer
                    int idx = in.readInt();
                    int from = in.readInt();
                    int type = in.readInt();
					
                    mFormat.setInteger(MediaFormat.KEY_SPU_FROM, from);
                    mFormat.setInteger(MediaFormat.KEY_SPU_TYPE, type);
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_TIMEDTEXT("+numOfField+"): " + mFormat.toString() + ", idx: " + idx);
                } else {
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_TIMEDTEXT("+numOfField+"): " + mFormat.toString());
                }
            } else if (mTrackType == MEDIA_TRACK_TYPE_VIDEO) {
                if (numOfField == 10) { //RTMediaPlayer
                    String type = mime;
                    mFormat = MediaFormat.createVideoFormat(type,
                            in.readInt(), in.readInt());
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    mFormat.setFloat(MediaFormat.KEY_FRAME_RATE, in.readFloat());
                    mFormat.setInteger(MediaFormat.KEY_BIT_RATE, in.readInt());
                    mFormat.setInteger(MediaFormat.KEY_IS_PROGRESSIVE, in.readInt());
                    mFormat.setInteger(MediaFormat.KEY_IS_MVC, in.readInt());
                    mFormat.setInteger(MediaFormat.KEY_IS_HDR, in.readInt());
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_VIDEO("+numOfField+"): " + mFormat.toString() + ", type: " + type);
                } else if (numOfField == 9) {   //NuPlayer extension
                    String type = mime;
                    mFormat = MediaFormat.createVideoFormat(type,
                            in.readInt(), in.readInt());
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    mFormat.setFloat(MediaFormat.KEY_FRAME_RATE, in.readFloat());
                    mFormat.setInteger(MediaFormat.KEY_IS_PROGRESSIVE, in.readInt());
                    mFormat.setInteger(MediaFormat.KEY_IS_HDR, in.readInt());
                    mFormat.setInteger(MediaFormat.KEY_IS_MVC, in.readInt());
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_VIDEO("+numOfField+"): " + mFormat.toString() + ", type: " + type);
                } else if (numOfField == 6) {   //NuPlayer
                    String type = mime;
                    mFormat = MediaFormat.createVideoFormat(type,
                            in.readInt(), in.readInt());
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    mFormat.setFloat(MediaFormat.KEY_FRAME_RATE, in.readFloat());
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_VIDEO("+numOfField+"): " + mFormat.toString() + ", type: " + type);
                } else {
                    mFormat = new MediaFormat();
                    mFormat.setString(MediaFormat.KEY_MIME, mime);
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_VIDEO ("+numOfField+"): " + mFormat.toString());
                }
            } else if (mTrackType == MEDIA_TRACK_TYPE_AUDIO) {
                if (numOfField == 8) {  //RTMediaPlayer
                    String type = mime;
                    mFormat = MediaFormat.createAudioFormat(type,
                            in.readInt(), in.readInt());
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    mFormat.setInteger(MediaFormat.KEY_BIT_RATE, in.readInt());
                    int idx = in.readInt();
                    int pid = in.readInt();
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_AUDIO("+numOfField+"): " + mFormat.toString() + ", type: " + type + ", idx: " + idx +
                            ", pid: " + pid);
                } else if (numOfField == 5) {   //NuPlayer extension
                    String type = mime;
                    mFormat = MediaFormat.createAudioFormat(type,
                            in.readInt(), in.readInt());
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_AUDIO("+numOfField+"): " + mFormat.toString() + ", type: " + type);
                } else {
                    mFormat = new MediaFormat();
                    mFormat.setString(MediaFormat.KEY_MIME, mime);
                    mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
                    Log.d(TAG, "got MEDIA_TRACK_TYPE_AUDIO("+numOfField+"): " + mFormat.toString());
                }
            } else {
                Log.d(TAG, "got " + mTrackType);
                mFormat = new MediaFormat();
                mFormat.setString(MediaFormat.KEY_MIME, mime);
                mFormat.setString(MediaFormat.KEY_LANGUAGE, language);
            }
        }

        /** @hide */
        TrackInfo(int type, MediaFormat format) {
            mTrackType = type;
            mFormat = format;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public int describeContents() {
            return 0;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeInt(mTrackType);
            dest.writeString(getLanguage());

            if (mTrackType == MEDIA_TRACK_TYPE_SUBTITLE) {
                dest.writeString(mFormat.getString(MediaFormat.KEY_MIME));
                dest.writeInt(mFormat.getInteger(MediaFormat.KEY_IS_AUTOSELECT));
                dest.writeInt(mFormat.getInteger(MediaFormat.KEY_IS_DEFAULT));
                dest.writeInt(mFormat.getInteger(MediaFormat.KEY_IS_FORCED_SUBTITLE));
            }
        }

        @Override
        public String toString() {
            StringBuilder out = new StringBuilder(128);
            out.append(getClass().getName());
            out.append('{');
            switch (mTrackType) {
            case MEDIA_TRACK_TYPE_VIDEO:
                out.append("VIDEO");
                break;
            case MEDIA_TRACK_TYPE_AUDIO:
                out.append("AUDIO");
                break;
            case MEDIA_TRACK_TYPE_TIMEDTEXT:
                out.append("TIMEDTEXT");
                break;
            case MEDIA_TRACK_TYPE_SUBTITLE:
                out.append("SUBTITLE");
                break;
            default:
                out.append("UNKNOWN");
                break;
            }
            out.append(", " + mFormat.toString());
            out.append("}");
            return out.toString();
        }

        /**
         * Used to read a TrackInfo from a Parcel.
         */
        static final Parcelable.Creator<TrackInfo> CREATOR
                = new Parcelable.Creator<TrackInfo>() {
                    @Override
                    public TrackInfo createFromParcel(Parcel in) {
                        return new TrackInfo(in);
                    }

                    @Override
                    public TrackInfo[] newArray(int size) {
                        return new TrackInfo[size];
                    }
                };

    };

    // We would like domain specific classes with more informative names than the `first` and `second`
    // in generic Pair, but we would also like to avoid creating new/trivial classes. As a compromise
    // we document the meanings of `first` and `second` here:
    //
    // Pair.first - inband track index; non-null iff representing an inband track.
    // Pair.second - a SubtitleTrack registered with mSubtitleController; non-null iff representing
    //               an inband subtitle track or any out-of-band track (subtitle or timedtext).
    private Vector<Pair<Integer, SubtitleTrack>> mIndexTrackPairs = new Vector<>();
    private BitSet mInbandTrackIndices = new BitSet();

    /**
     * Returns an array of track information.
     *
     * @return Array of track info. The total number of tracks is the array length.
     * Must be called again if an external timed text source has been added after any of the
     * addTimedTextSource methods are called.
     * @throws IllegalStateException if it is called in an invalid state.
     */
    public TrackInfo[] getTrackInfo() throws IllegalStateException {
        scanInternalSubtitleTracks();
        TrackInfo trackInfo[] = getInbandTrackInfo();
        // add out-of-band tracks
        synchronized (mIndexTrackPairs) {
            TrackInfo allTrackInfo[] = new TrackInfo[mIndexTrackPairs.size()];
            for (int i = 0; i < allTrackInfo.length; i++) {
                Pair<Integer, SubtitleTrack> p = mIndexTrackPairs.get(i);
                if (p.first != null) {
                    if(p.first >= trackInfo.length) {
                        allTrackInfo[i] = new TrackInfo(TrackInfo.MEDIA_TRACK_TYPE_UNKNOWN, null);
                        Log.w(TAG, "ArrayIndexOutOfBounds: " + p.first +"/"+trackInfo.length);
                        continue;
                    }
                    try {
                        // inband track
                        allTrackInfo[i] = trackInfo[p.first];
                    } catch (ArrayIndexOutOfBoundsException e) {
                        Log.w(TAG, "Exception with trackInfo: " + e);
                        return new TrackInfo[0];
                    }
                } else {
                    SubtitleTrack track = p.second;
                    allTrackInfo[i] = new TrackInfo(track.getTrackType(), track.getFormat());
                }
            }
            return allTrackInfo;
        }
    }

    public void resetTrackInfo() {
        synchronized (mIndexTrackPairs) {
            mIndexTrackPairs.clear();
            mInbandTrackIndices.clear();
        };
    }

    private TrackInfo[] getInbandTrackInfo() throws IllegalStateException {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_GET_TRACK_INFO);
            invoke(request, reply);
            TrackInfo trackInfo[] = reply.createTypedArray(TrackInfo.CREATOR);
            return trackInfo;
        } catch (RuntimeException e) {
            Log.w(TAG, "Illegal state to get track info: " + e);
            TrackInfo trackInfo[] = new TrackInfo[0];
            return trackInfo;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /* Do not change these values without updating their counterparts
     * in include/media/stagefright/MediaDefs.h and media/libstagefright/MediaDefs.cpp!
     */
    /**
     * MIME type for SubRip (SRT) container. Used in addTimedTextSource APIs.
     */
    public static final String MEDIA_MIMETYPE_TEXT_SUBRIP = "application/x-subrip";

    /**
     * MIME type for WebVTT subtitle data.
     * @hide
     */
    public static final String MEDIA_MIMETYPE_TEXT_VTT = "text/vtt";

    /**
     * MIME type for CEA-608 closed caption data.
     * @hide
     */
    public static final String MEDIA_MIMETYPE_TEXT_CEA_608 = "text/cea-608";

    /**
     * MIME type for CEA-708 closed caption data.
     * @hide
     */
    public static final String MEDIA_MIMETYPE_TEXT_CEA_708 = "text/cea-708";

    /*
     * A helper function to check if the mime type is supported by media framework.
     */
    private static boolean availableMimeTypeForExternalSource(String mimeType) {
        if (MEDIA_MIMETYPE_TEXT_SUBRIP.equals(mimeType)) {
            return true;
        }
        return false;
    }

    private SubtitleController mSubtitleController;

    /** @hide */
    public void setSubtitleAnchor(
            SubtitleController controller,
            SubtitleController.Anchor anchor) {
        // TODO: create SubtitleController in MediaPlayer
        mSubtitleController = controller;
        mSubtitleController.setAnchor(anchor);
    }

    /**
     * The private version of setSubtitleAnchor is used internally to set mSubtitleController if
     * necessary when clients don't provide their own SubtitleControllers using the public version
     * {@link #setSubtitleAnchor(SubtitleController, Anchor)} (e.g. {@link VideoView} provides one).
     */
    private synchronized void setSubtitleAnchor() {
        if (mSubtitleController == null) {
            final HandlerThread thread = new HandlerThread("SetSubtitleAnchorThread");
            thread.start();
            Handler handler = new Handler(thread.getLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    Context context = ActivityThread.currentApplication();
                    mSubtitleController = new SubtitleController(context, mTimeProvider, MediaPlayer.this);
                    mSubtitleController.setAnchor(new Anchor() {
                        @Override
                        public void setSubtitleWidget(RenderingWidget subtitleWidget) {
                        }

                        @Override
                        public Looper getSubtitleLooper() {
                            return Looper.getMainLooper();
                        }
                    });
                    thread.getLooper().quitSafely();
                }
            });
            try {
                thread.join();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                Log.w(TAG, "failed to join SetSubtitleAnchorThread");
            }
        }
    }

    private int mSelectedSubtitleTrackIndex = -1;
    private Vector<InputStream> mOpenSubtitleSources;

    private OnSubtitleDataListener mSubtitleDataListener = new OnSubtitleDataListener() {
        @Override
        public void onSubtitleData(MediaPlayer mp, SubtitleData data) {
            int index = data.getTrackIndex();
            synchronized (mIndexTrackPairs) {
                for (Pair<Integer, SubtitleTrack> p : mIndexTrackPairs) {
                    if (p.first != null && p.first == index && p.second != null) {
                        // inband subtitle track that owns data
                        SubtitleTrack track = p.second;
                        track.onData(data);
                    }
                }
            }
        }
    };

    /** @hide */
    @Override
    public void onSubtitleTrackSelected(SubtitleTrack track) {
        if (mSelectedSubtitleTrackIndex >= 0) {
            try {
                selectOrDeselectInbandTrack(mSelectedSubtitleTrackIndex, false);
            } catch (IllegalStateException e) {
            }
            mSelectedSubtitleTrackIndex = -1;
        }
        setOnSubtitleDataListener(null);
        if (track == null) {
            return;
        }

        synchronized (mIndexTrackPairs) {
            for (Pair<Integer, SubtitleTrack> p : mIndexTrackPairs) {
                if (p.first != null && p.second == track) {
                    // inband subtitle track that is selected
                    mSelectedSubtitleTrackIndex = p.first;
                    break;
                }
            }
        }

        if (mSelectedSubtitleTrackIndex >= 0) {
            try {
                selectOrDeselectInbandTrack(mSelectedSubtitleTrackIndex, true);
            } catch (IllegalStateException e) {
            }
            setOnSubtitleDataListener(mSubtitleDataListener);
        }
        // no need to select out-of-band tracks
    }

    /** @hide */
    public void addSubtitleSource(InputStream is, MediaFormat format)
            throws IllegalStateException
    {
        final InputStream fIs = is;
        final MediaFormat fFormat = format;

        if (is != null) {
            // Ensure all input streams are closed.  It is also a handy
            // way to implement timeouts in the future.
            synchronized(mOpenSubtitleSources) {
                mOpenSubtitleSources.add(is);
            }
        } else {
            Log.w(TAG, "addSubtitleSource called with null InputStream");
        }

        getMediaTimeProvider();

        // process each subtitle in its own thread
        final HandlerThread thread = new HandlerThread("SubtitleReadThread",
              Process.THREAD_PRIORITY_BACKGROUND + Process.THREAD_PRIORITY_MORE_FAVORABLE);
        thread.start();
        Handler handler = new Handler(thread.getLooper());
        handler.post(new Runnable() {
            private int addTrack() {
                if (fIs == null || mSubtitleController == null) {
                    return MEDIA_INFO_UNSUPPORTED_SUBTITLE;
                }

                SubtitleTrack track = mSubtitleController.addTrack(fFormat);
                if (track == null) {
                    return MEDIA_INFO_UNSUPPORTED_SUBTITLE;
                }

                // TODO: do the conversion in the subtitle track
                Scanner scanner = new Scanner(fIs, "UTF-8");
                String contents = scanner.useDelimiter("\\A").next();
                synchronized(mOpenSubtitleSources) {
                    mOpenSubtitleSources.remove(fIs);
                }
                scanner.close();
                synchronized (mIndexTrackPairs) {
                    mIndexTrackPairs.add(Pair.<Integer, SubtitleTrack>create(null, track));
                }
                Handler h = mTimeProvider.mEventHandler;
                int what = TimeProvider.NOTIFY;
                int arg1 = TimeProvider.NOTIFY_TRACK_DATA;
                Pair<SubtitleTrack, byte[]> trackData = Pair.create(track, contents.getBytes());
                Message m = h.obtainMessage(what, arg1, 0, trackData);
                h.sendMessage(m);
                return MEDIA_INFO_EXTERNAL_METADATA_UPDATE;
            }

            public void run() {
                int res = addTrack();
                if (mEventHandler != null) {
                    Message m = mEventHandler.obtainMessage(MEDIA_INFO, res, 0, null);
                    mEventHandler.sendMessage(m);
                }
                thread.getLooper().quitSafely();
            }
        });
    }

    private void scanInternalSubtitleTracks() {
        if (mSubtitleController == null) {
            Log.d(TAG, "setSubtitleAnchor in MediaPlayer");
            setSubtitleAnchor();
        }

        populateInbandTracks();

        if (mSubtitleController != null) {
            mSubtitleController.selectDefaultTrack();
        }
    }

    private void populateInbandTracks() {
        TrackInfo[] tracks = getInbandTrackInfo();
        synchronized (mIndexTrackPairs) {
            for (int i = 0; i < tracks.length; i++) {
                if (mInbandTrackIndices.get(i)) {
                    continue;
                } else {
                    mInbandTrackIndices.set(i);
                }

                // newly appeared inband track
                if (tracks[i].getTrackType() == TrackInfo.MEDIA_TRACK_TYPE_SUBTITLE) {
                    SubtitleTrack track = mSubtitleController.addTrack(
                            tracks[i].getFormat());
                    mIndexTrackPairs.add(Pair.create(i, track));
                } else {
                    mIndexTrackPairs.add(Pair.<Integer, SubtitleTrack>create(i, null));
                }
            }
        }
    }

    /* TODO: Limit the total number of external timed text source to a reasonable number.
     */
    /**
     * Adds an external timed text source file.
     *
     * Currently supported format is SubRip with the file extension .srt, case insensitive.
     * Note that a single external timed text source may contain multiple tracks in it.
     * One can find the total number of available tracks using {@link #getTrackInfo()} to see what
     * additional tracks become available after this method call.
     *
     * @param path The file path of external timed text source file.
     * @param mimeType The mime type of the file. Must be one of the mime types listed above.
     * @throws IOException if the file cannot be accessed or is corrupted.
     * @throws IllegalArgumentException if the mimeType is not supported.
     * @throws IllegalStateException if called in an invalid state.
     */
    public void addTimedTextSource(String path, String mimeType)
            throws IOException, IllegalArgumentException, IllegalStateException {
        if (!availableMimeTypeForExternalSource(mimeType)) {
            final String msg = "Illegal mimeType for timed text source: " + mimeType;
            throw new IllegalArgumentException(msg);
        }

        File file = new File(path);
        if (file.exists()) {
            FileInputStream is = new FileInputStream(file);
            FileDescriptor fd = is.getFD();
            addTimedTextSource(fd, mimeType);
            is.close();
        } else {
            // We do not support the case where the path is not a file.
            throw new IOException(path);
        }
    }

    /**
     * Adds an external timed text source file (Uri).
     *
     * Currently supported format is SubRip with the file extension .srt, case insensitive.
     * Note that a single external timed text source may contain multiple tracks in it.
     * One can find the total number of available tracks using {@link #getTrackInfo()} to see what
     * additional tracks become available after this method call.
     *
     * @param context the Context to use when resolving the Uri
     * @param uri the Content URI of the data you want to play
     * @param mimeType The mime type of the file. Must be one of the mime types listed above.
     * @throws IOException if the file cannot be accessed or is corrupted.
     * @throws IllegalArgumentException if the mimeType is not supported.
     * @throws IllegalStateException if called in an invalid state.
     */
    public void addTimedTextSource(Context context, Uri uri, String mimeType)
            throws IOException, IllegalArgumentException, IllegalStateException {
        String scheme = uri.getScheme();
        if(scheme == null || scheme.equals("file")) {
            addTimedTextSource(uri.getPath(), mimeType);
            return;
        }

        AssetFileDescriptor fd = null;
        try {
            ContentResolver resolver = context.getContentResolver();
            fd = resolver.openAssetFileDescriptor(uri, "r");
            if (fd == null) {
                return;
            }
            addTimedTextSource(fd.getFileDescriptor(), mimeType);
            return;
        } catch (SecurityException ex) {
        } catch (IOException ex) {
        } finally {
            if (fd != null) {
                fd.close();
            }
        }
    }

    /**
     * Adds an external timed text source file (FileDescriptor).
     *
     * It is the caller's responsibility to close the file descriptor.
     * It is safe to do so as soon as this call returns.
     *
     * Currently supported format is SubRip. Note that a single external timed text source may
     * contain multiple tracks in it. One can find the total number of available tracks
     * using {@link #getTrackInfo()} to see what additional tracks become available
     * after this method call.
     *
     * @param fd the FileDescriptor for the file you want to play
     * @param mimeType The mime type of the file. Must be one of the mime types listed above.
     * @throws IllegalArgumentException if the mimeType is not supported.
     * @throws IllegalStateException if called in an invalid state.
     */
    public void addTimedTextSource(FileDescriptor fd, String mimeType)
            throws IllegalArgumentException, IllegalStateException {
        // intentionally less than LONG_MAX
        addTimedTextSource(fd, 0, 0x7ffffffffffffffL, mimeType);
    }

    /**
     * Adds an external timed text file (FileDescriptor).
     *
     * It is the caller's responsibility to close the file descriptor.
     * It is safe to do so as soon as this call returns.
     *
     * Currently supported format is SubRip. Note that a single external timed text source may
     * contain multiple tracks in it. One can find the total number of available tracks
     * using {@link #getTrackInfo()} to see what additional tracks become available
     * after this method call.
     *
     * @param fd the FileDescriptor for the file you want to play
     * @param offset the offset into the file where the data to be played starts, in bytes
     * @param length the length in bytes of the data to be played
     * @param mime The mime type of the file. Must be one of the mime types listed above.
     * @throws IllegalArgumentException if the mimeType is not supported.
     * @throws IllegalStateException if called in an invalid state.
     */
    public void addTimedTextSource(FileDescriptor fd, long offset, long length, String mime)
            throws IllegalArgumentException, IllegalStateException {
        if (!availableMimeTypeForExternalSource(mime)) {
            throw new IllegalArgumentException("Illegal mimeType for timed text source: " + mime);
        }

        FileDescriptor fd2;
        try {
            fd2 = Libcore.os.dup(fd);
        } catch (ErrnoException ex) {
            Log.e(TAG, ex.getMessage(), ex);
            throw new RuntimeException(ex);
        }

        final MediaFormat fFormat = new MediaFormat();
        fFormat.setString(MediaFormat.KEY_MIME, mime);
        fFormat.setInteger(MediaFormat.KEY_IS_TIMED_TEXT, 1);

        // A MediaPlayer created by a VideoView should already have its mSubtitleController set.
        if (mSubtitleController == null) {
            setSubtitleAnchor();
        }

        if (!mSubtitleController.hasRendererFor(fFormat)) {
            // test and add not atomic
            Context context = ActivityThread.currentApplication();
            mSubtitleController.registerRenderer(new SRTRenderer(context, mEventHandler));
        }
        final SubtitleTrack track = mSubtitleController.addTrack(fFormat);
        synchronized (mIndexTrackPairs) {
            mIndexTrackPairs.add(Pair.<Integer, SubtitleTrack>create(null, track));
        }

        getMediaTimeProvider();

        final FileDescriptor fd3 = fd2;
        final long offset2 = offset;
        final long length2 = length;
        final HandlerThread thread = new HandlerThread(
                "TimedTextReadThread",
                Process.THREAD_PRIORITY_BACKGROUND + Process.THREAD_PRIORITY_MORE_FAVORABLE);
        thread.start();
        Handler handler = new Handler(thread.getLooper());
        handler.post(new Runnable() {
            private int addTrack() {
                InputStream is = null;
                final ByteArrayOutputStream bos = new ByteArrayOutputStream();
                try {
                    Libcore.os.lseek(fd3, offset2, OsConstants.SEEK_SET);
                    byte[] buffer = new byte[4096];
                    for (long total = 0; total < length2;) {
                        int bytesToRead = (int) Math.min(buffer.length, length2 - total);
                        int bytes = IoBridge.read(fd3, buffer, 0, bytesToRead);
                        if (bytes < 0) {
                            break;
                        } else {
                            bos.write(buffer, 0, bytes);
                            total += bytes;
                        }
                    }
                    Handler h = mTimeProvider.mEventHandler;
                    int what = TimeProvider.NOTIFY;
                    int arg1 = TimeProvider.NOTIFY_TRACK_DATA;
                    Pair<SubtitleTrack, byte[]> trackData = Pair.create(track, bos.toByteArray());
                    Message m = h.obtainMessage(what, arg1, 0, trackData);
                    h.sendMessage(m);
                    return MEDIA_INFO_EXTERNAL_METADATA_UPDATE;
                } catch (Exception e) {
                    Log.e(TAG, e.getMessage(), e);
                    return MEDIA_INFO_TIMED_TEXT_ERROR;
                } finally {
                    if (is != null) {
                        try {
                            is.close();
                        } catch (IOException e) {
                            Log.e(TAG, e.getMessage(), e);
                        }
                    }
                }
            }

            public void run() {
                int res = addTrack();
                if (mEventHandler != null) {
                    Message m = mEventHandler.obtainMessage(MEDIA_INFO, res, 0, null);
                    mEventHandler.sendMessage(m);
                }
                thread.getLooper().quitSafely();
            }
        });
    }

    /**
     * Returns the index of the audio, video, or subtitle track currently selected for playback,
     * The return value is an index into the array returned by {@link #getTrackInfo()}, and can
     * be used in calls to {@link #selectTrack(int)} or {@link #deselectTrack(int)}.
     *
     * @param trackType should be one of {@link TrackInfo#MEDIA_TRACK_TYPE_VIDEO},
     * {@link TrackInfo#MEDIA_TRACK_TYPE_AUDIO}, or
     * {@link TrackInfo#MEDIA_TRACK_TYPE_SUBTITLE}
     * @return index of the audio, video, or subtitle track currently selected for playback;
     * a negative integer is returned when there is no selected track for {@code trackType} or
     * when {@code trackType} is not one of audio, video, or subtitle.
     * @throws IllegalStateException if called after {@link #release()}
     *
     * @see #getTrackInfo()
     * @see #selectTrack(int)
     * @see #deselectTrack(int)
     */
    public int getSelectedTrack(int trackType) throws IllegalStateException {
        if (mSubtitleController != null
                && (trackType == TrackInfo.MEDIA_TRACK_TYPE_SUBTITLE
                || trackType == TrackInfo.MEDIA_TRACK_TYPE_TIMEDTEXT)) {
            SubtitleTrack subtitleTrack = mSubtitleController.getSelectedTrack();
            if (subtitleTrack != null) {
                synchronized (mIndexTrackPairs) {
                    for (int i = 0; i < mIndexTrackPairs.size(); i++) {
                        Pair<Integer, SubtitleTrack> p = mIndexTrackPairs.get(i);
                        if (p.second == subtitleTrack && subtitleTrack.getTrackType() == trackType) {
                            return i;
                        }
                    }
                }
            }
        }

        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(INVOKE_ID_GET_SELECTED_TRACK);
            request.writeInt(trackType);
            invoke(request, reply);
            int inbandTrackIndex = reply.readInt();
            synchronized (mIndexTrackPairs) {
                for (int i = 0; i < mIndexTrackPairs.size(); i++) {
                    Pair<Integer, SubtitleTrack> p = mIndexTrackPairs.get(i);
                    if (p.first != null && p.first == inbandTrackIndex) {
                        return i;
                    }
                }
            }
            return -1;
        } finally {
            request.recycle();
            reply.recycle();
        }
    }

    /**
     * Selects a track.
     * <p>
     * If a MediaPlayer is in invalid state, it throws an IllegalStateException exception.
     * If a MediaPlayer is in <em>Started</em> state, the selected track is presented immediately.
     * If a MediaPlayer is not in Started state, it just marks the track to be played.
     * </p>
     * <p>
     * In any valid state, if it is called multiple times on the same type of track (ie. Video,
     * Audio, Timed Text), the most recent one will be chosen.
     * </p>
     * <p>
     * The first audio and video tracks are selected by default if available, even though
     * this method is not called. However, no timed text track will be selected until
     * this function is called.
     * </p>
     * <p>
     * Currently, only timed text tracks or audio tracks can be selected via this method.
     * In addition, the support for selecting an audio track at runtime is pretty limited
     * in that an audio track can only be selected in the <em>Prepared</em> state.
     * </p>
     * @param index the index of the track to be selected. The valid range of the index
     * is 0..total number of track - 1. The total number of tracks as well as the type of
     * each individual track can be found by calling {@link #getTrackInfo()} method.
     * @throws IllegalStateException if called in an invalid state.
     *
     * @see android.media.MediaPlayer#getTrackInfo
     */
    public void selectTrack(int index) throws IllegalStateException {
        selectOrDeselectTrack(index, true /* select */);
    }

    /**
     * Deselect a track.
     * <p>
     * Currently, the track must be a timed text track and no audio or video tracks can be
     * deselected. If the timed text track identified by index has not been
     * selected before, it throws an exception.
     * </p>
     * @param index the index of the track to be deselected. The valid range of the index
     * is 0..total number of tracks - 1. The total number of tracks as well as the type of
     * each individual track can be found by calling {@link #getTrackInfo()} method.
     * @throws IllegalStateException if called in an invalid state.
     *
     * @see android.media.MediaPlayer#getTrackInfo
     */
    public void deselectTrack(int index) throws IllegalStateException {
        selectOrDeselectTrack(index, false /* select */);
    }

    private void selectOrDeselectTrack(int index, boolean select)
            throws IllegalStateException {
        // handle subtitle track through subtitle controller
        populateInbandTracks();

        Pair<Integer,SubtitleTrack> p = null;
        try {
            p = mIndexTrackPairs.get(index);
        } catch (ArrayIndexOutOfBoundsException e) {
            // ignore bad index
            return;
        }

        SubtitleTrack track = p.second;
        if (track == null) {
            // inband (de)select
            selectOrDeselectInbandTrack(p.first, select);
            return;
        }

        if (mSubtitleController == null) {
            return;
        }

        if (!select) {
            // out-of-band deselect
            if (mSubtitleController.getSelectedTrack() == track) {
                mSubtitleController.selectTrack(null);
            } else {
                Log.w(TAG, "trying to deselect track that was not selected");
                throw new RuntimeException("trying to deselect track that was not selected");
            }
            return;
        }

        // out-of-band select
        if (track.getTrackType() == TrackInfo.MEDIA_TRACK_TYPE_TIMEDTEXT) {
            int ttIndex = getSelectedTrack(TrackInfo.MEDIA_TRACK_TYPE_TIMEDTEXT);
            synchronized (mIndexTrackPairs) {
                if (ttIndex >= 0 && ttIndex < mIndexTrackPairs.size()) {
                    Pair<Integer,SubtitleTrack> p2 = mIndexTrackPairs.get(ttIndex);
                    if (p2.first != null && p2.second == null) {
                        // deselect inband counterpart
                        selectOrDeselectInbandTrack(p2.first, false);
                    }
                }
            }
        }
        mSubtitleController.selectTrack(track);
    }

    private void selectOrDeselectInbandTrack(int index, boolean select)
            throws IllegalStateException {
        Parcel request = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            request.writeInterfaceToken(IMEDIA_PLAYER);
            request.writeInt(select? INVOKE_ID_SELECT_TRACK: INVOKE_ID_DESELECT_TRACK);
            request.writeInt(index);
            invoke(request, reply);
        } finally {
            request.recycle();
            reply.recycle();
        }
    }


    /**
     * @param reply Parcel with audio/video duration info for battery
                    tracking usage
     * @return The status code.
     * {@hide}
     */
    public native static int native_pullBatteryData(Parcel reply);

    /**
     * Sets the target UDP re-transmit endpoint for the low level player.
     * Generally, the address portion of the endpoint is an IP multicast
     * address, although a unicast address would be equally valid.  When a valid
     * retransmit endpoint has been set, the media player will not decode and
     * render the media presentation locally.  Instead, the player will attempt
     * to re-multiplex its media data using the Android@Home RTP profile and
     * re-transmit to the target endpoint.  Receiver devices (which may be
     * either the same as the transmitting device or different devices) may
     * instantiate, prepare, and start a receiver player using a setDataSource
     * URL of the form...
     *
     * aahRX://&lt;multicastIP&gt;:&lt;port&gt;
     *
     * to receive, decode and render the re-transmitted content.
     *
     * setRetransmitEndpoint may only be called before setDataSource has been
     * called; while the player is in the Idle state.
     *
     * @param endpoint the address and UDP port of the re-transmission target or
     * null if no re-transmission is to be performed.
     * @throws IllegalStateException if it is called in an invalid state
     * @throws IllegalArgumentException if the retransmit endpoint is supplied,
     * but invalid.
     *
     * {@hide} pending API council
     */
    public void setRetransmitEndpoint(InetSocketAddress endpoint)
            throws IllegalStateException, IllegalArgumentException
    {
        String addrString = null;
        int port = 0;

        if (null != endpoint) {
            addrString = endpoint.getAddress().getHostAddress();
            port = endpoint.getPort();
        }

        int ret = native_setRetransmitEndpoint(addrString, port);
        if (ret != 0) {
            throw new IllegalArgumentException("Illegal re-transmit endpoint; native ret " + ret);
        }
    }

    private native final int native_setRetransmitEndpoint(String addrString, int port);

    @Override
    protected void finalize() {
        baseRelease();
        native_finalize();
    }

    /* Do not change these values without updating their counterparts
     * in include/media/mediaplayer.h!
     */
    private static final int MEDIA_NOP = 0; // interface test message
    private static final int MEDIA_PREPARED = 1;
    private static final int MEDIA_PLAYBACK_COMPLETE = 2;
    private static final int MEDIA_BUFFERING_UPDATE = 3;
    private static final int MEDIA_SEEK_COMPLETE = 4;
    private static final int MEDIA_SET_VIDEO_SIZE = 5;
    private static final int MEDIA_STARTED = 6;
    private static final int MEDIA_PAUSED = 7;
    private static final int MEDIA_STOPPED = 8;
    private static final int MEDIA_SKIPPED = 9;
    private static final int MEDIA_TIMED_TEXT = 99;
    private static final int MEDIA_ERROR = 100;
    private static final int MEDIA_INFO = 200;
    private static final int MEDIA_SUBTITLE_DATA = 201;
    private static final int MEDIA_META_DATA = 202;

    //RTK proprietary event types
    private static final int MEDIA_SPEED_CHANGE = 20151010;
    private static final int MEDIA_SEEK_RESULT = 20151011;

    private TimeProvider mTimeProvider;

    /** @hide */
    public MediaTimeProvider getMediaTimeProvider() {
        if (mTimeProvider == null) {
            mTimeProvider = new TimeProvider(this);
        }
        return mTimeProvider;
    }

    private class EventHandler extends Handler
    {
        private MediaPlayer mMediaPlayer;

        public EventHandler(MediaPlayer mp, Looper looper) {
            super(looper);
            mMediaPlayer = mp;
        }

        @Override
        public void handleMessage(Message msg) {
            if (mMediaPlayer.mNativeContext == 0) {
                Log.w(TAG, "mediaplayer went away with unhandled events");
                return;
            }
            switch(msg.what) {
            case MEDIA_PREPARED:
                try {
                    scanInternalSubtitleTracks();
                } catch (RuntimeException e) {
                    // send error message instead of crashing;
                    // send error message instead of inlining a call to onError
                    // to avoid code duplication.
                    Message msg2 = obtainMessage(
                            MEDIA_ERROR, MEDIA_ERROR_UNKNOWN, MEDIA_ERROR_UNSUPPORTED, null);
                    sendMessage(msg2);
                }
                OnPreparedListener onPreparedListener = mOnPreparedListener;
                if (onPreparedListener != null)
                    onPreparedListener.onPrepared(mMediaPlayer);
                return;

            case MEDIA_PLAYBACK_COMPLETE:
// add by RTK
                if (ENABLE_RT_MEDIAPLAYER) {
                    stopMeasureThread();
                }
// end of added by RTK
                {
                    OnCompletionListener onCompletionListener = mOnCompletionListener;
                    if (onCompletionListener != null)
                        onCompletionListener.onCompletion(mMediaPlayer);
                }
                stayAwake(false);
                return;

            case MEDIA_STOPPED:
                {
                    TimeProvider timeProvider = mTimeProvider;
                    if (timeProvider != null) {
                        timeProvider.onStopped();
                    }
                }
                break;

            case MEDIA_STARTED:
            case MEDIA_PAUSED:
                {
                    TimeProvider timeProvider = mTimeProvider;
                    if (timeProvider != null) {
                        timeProvider.onPaused(msg.what == MEDIA_PAUSED);
                    }
                }
                break;

            case MEDIA_BUFFERING_UPDATE:
                OnBufferingUpdateListener onBufferingUpdateListener = mOnBufferingUpdateListener;
                if (onBufferingUpdateListener != null)
                    onBufferingUpdateListener.onBufferingUpdate(mMediaPlayer, msg.arg1);
                return;

            case MEDIA_SEEK_COMPLETE:
                OnSeekCompleteListener onSeekCompleteListener = mOnSeekCompleteListener;
                if (onSeekCompleteListener != null) {
                    onSeekCompleteListener.onSeekComplete(mMediaPlayer);
                }
                // fall through

            case MEDIA_SKIPPED:
                {
                    TimeProvider timeProvider = mTimeProvider;
                    if (timeProvider != null) {
                        timeProvider.onSeekComplete(mMediaPlayer);
                    }
                }
                return;

            case MEDIA_SET_VIDEO_SIZE:
                OnVideoSizeChangedListener onVideoSizeChangedListener = mOnVideoSizeChangedListener;
                if (onVideoSizeChangedListener != null) {
                    onVideoSizeChangedListener.onVideoSizeChanged(
                        mMediaPlayer, msg.arg1, msg.arg2);
                }
                return;

            case MEDIA_SPEED_CHANGE:
                Log.i(TAG, "Speed (" + msg.arg1 + ")");
                if (mOnSpeedChangedListener != null)
                    mOnSpeedChangedListener.onSpeedChanged(mMediaPlayer, msg.arg1);
                return;

            case MEDIA_ERROR:
// added by RTK
                if (ENABLE_RT_MEDIAPLAYER) {
                    stopMeasureThread();
                }
// end of add by RTK
                Log.e(TAG, "Error (" + msg.arg1 + "," + msg.arg2 + ")");
                boolean error_was_handled = false;
                OnErrorListener onErrorListener = mOnErrorListener;
                if (onErrorListener != null) {
                    error_was_handled = onErrorListener.onError(mMediaPlayer, msg.arg1, msg.arg2);
                }
                {
                    OnCompletionListener onCompletionListener = mOnCompletionListener;
                    if (onCompletionListener != null && ! error_was_handled) {
                        onCompletionListener.onCompletion(mMediaPlayer);
                    }
                }
                stayAwake(false);
                return;

            case MEDIA_INFO:
                switch (msg.arg1) {
                case MEDIA_INFO_VIDEO_TRACK_LAGGING:
                    Log.i(TAG, "Info (" + msg.arg1 + "," + msg.arg2 + ")");
                    break;
                case MEDIA_INFO_METADATA_UPDATE:
                    try {
                        scanInternalSubtitleTracks();
                    } catch (RuntimeException e) {
                        Message msg2 = obtainMessage(
                                MEDIA_ERROR, MEDIA_ERROR_UNKNOWN, MEDIA_ERROR_UNSUPPORTED, null);
                        sendMessage(msg2);
                    }
                    // fall through

                case MEDIA_INFO_EXTERNAL_METADATA_UPDATE:
                    msg.arg1 = MEDIA_INFO_METADATA_UPDATE;
                    // update default track selection
                    if (mSubtitleController != null) {
                        mSubtitleController.selectDefaultTrack();
                    }
                    break;
                case MEDIA_INFO_BUFFERING_START:
                case MEDIA_INFO_BUFFERING_END:
                    TimeProvider timeProvider = mTimeProvider;
                    if (timeProvider != null) {
                        timeProvider.onBuffering(msg.arg1 == MEDIA_INFO_BUFFERING_START);
                    }
                    break;
// added by RTK
                // Manipulate the data for calculating download speed. This implementation
                // here has to match the implementation in RTMediaPlayer.cpp as well
                case MEDIA_INFO_NETWORK_BANDWIDTH:
                    if (ENABLE_RT_MEDIAPLAYER) {
                         msg.arg2 = kbytesPerSecond;
                    }
                    break;
                case MEDIA_INFO_DOMAIN_UPDATE:
                case MEDIA_INFO_TITLE_UPDATE:
                    resetTrackInfo();
                    break;
// end of added by RTK
                }

                OnInfoListener onInfoListener = mOnInfoListener;
                if (onInfoListener != null) {
                    onInfoListener.onInfo(mMediaPlayer, msg.arg1, msg.arg2);
                }
                // No real default action so far.
                return;
            case MEDIA_TIMED_TEXT:
                OnTimedTextListener onTimedTextListener = mOnTimedTextListener;
                if (onTimedTextListener == null)
                    return;
                if (msg.obj == null) {
                    onTimedTextListener.onTimedText(mMediaPlayer, null);
                } else {
                    if (msg.obj instanceof Parcel) {
                        Parcel parcel = (Parcel)msg.obj;
                        TimedText text = new TimedText(parcel);
                        parcel.recycle();
                        onTimedTextListener.onTimedText(mMediaPlayer, text);
                    }
                }
                return;

            case MEDIA_SUBTITLE_DATA:
                OnSubtitleDataListener onSubtitleDataListener = mOnSubtitleDataListener;
                if (onSubtitleDataListener == null) {
                    return;
                }
                if (msg.obj instanceof Parcel) {
                    Parcel parcel = (Parcel) msg.obj;
                    SubtitleData data = new SubtitleData(parcel);
                    parcel.recycle();
                    onSubtitleDataListener.onSubtitleData(mMediaPlayer, data);
                }
                return;

            case MEDIA_META_DATA:
                OnTimedMetaDataAvailableListener onTimedMetaDataAvailableListener =
                    mOnTimedMetaDataAvailableListener;
                if (onTimedMetaDataAvailableListener == null) {
                    return;
                }
                if (msg.obj instanceof Parcel) {
                    Parcel parcel = (Parcel) msg.obj;
                    TimedMetaData data = TimedMetaData.createTimedMetaDataFromParcel(parcel);
                    parcel.recycle();
                    onTimedMetaDataAvailableListener.onTimedMetaDataAvailable(mMediaPlayer, data);
                }
                return;

            case MEDIA_NOP: // interface test message - ignore
                break;

            case MEDIA_SEEK_RESULT:
                Log.i(TAG, "Seek result (" + msg.arg1 + ")");
                if (mOnSeekResultListener != null)
                    mOnSeekResultListener.onSeekResult(mMediaPlayer, msg.arg1);
                break;

            default:
                Log.e(TAG, "Unknown message type " + msg.what);
                return;
            }
        }
    }

    /*
     * Called from native code when an interesting event happens.  This method
     * just uses the EventHandler system to post the event back to the main app thread.
     * We use a weak reference to the original MediaPlayer object so that the native
     * code is safe from the object disappearing from underneath it.  (This is
     * the cookie passed to native_setup().)
     */
    private static void postEventFromNative(Object mediaplayer_ref,
                                            int what, int arg1, int arg2, Object obj)
    {
        MediaPlayer mp = (MediaPlayer)((WeakReference)mediaplayer_ref).get();
        if (mp == null) {
            return;
        }

        if (what == MEDIA_INFO && arg1 == MEDIA_INFO_STARTED_AS_NEXT) {
            // this acquires the wakelock if needed, and sets the client side state
            mp.start();
        }
        if (mp.mEventHandler != null) {
            Message m = mp.mEventHandler.obtainMessage(what, arg1, arg2, obj);
            mp.mEventHandler.sendMessage(m);
        }
    }

    /**
     * Interface definition for a callback to be invoked when the media
     * source is ready for playback.
     */
    public interface OnPreparedListener
    {
        /**
         * Called when the media file is ready for playback.
         *
         * @param mp the MediaPlayer that is ready for playback
         */
        void onPrepared(MediaPlayer mp);
    }

    /**
     * Register a callback to be invoked when the media source is ready
     * for playback.
     *
     * @param listener the callback that will be run
     */
    public void setOnPreparedListener(OnPreparedListener listener)
    {
        mOnPreparedListener = listener;
    }

    private OnPreparedListener mOnPreparedListener;

    /**
     * Interface definition for a callback to be invoked when playback of
     * a media source has completed.
     */
    public interface OnCompletionListener
    {
        /**
         * Called when the end of a media source is reached during playback.
         *
         * @param mp the MediaPlayer that reached the end of the file
         */
        void onCompletion(MediaPlayer mp);
    }

    /**
     * Register a callback to be invoked when the end of a media source
     * has been reached during playback.
     *
     * @param listener the callback that will be run
     */
    public void setOnCompletionListener(OnCompletionListener listener)
    {
        mOnCompletionListener = listener;
    }

    private OnCompletionListener mOnCompletionListener;

    /**
     * Interface definition of a callback to be invoked indicating buffering
     * status of a media resource being streamed over the network.
     */
    public interface OnBufferingUpdateListener
    {
        /**
         * Called to update status in buffering a media stream received through
         * progressive HTTP download. The received buffering percentage
         * indicates how much of the content has been buffered or played.
         * For example a buffering update of 80 percent when half the content
         * has already been played indicates that the next 30 percent of the
         * content to play has been buffered.
         *
         * @param mp      the MediaPlayer the update pertains to
         * @param percent the percentage (0-100) of the content
         *                that has been buffered or played thus far
         */
        void onBufferingUpdate(MediaPlayer mp, int percent);
    }

    /**
     * Register a callback to be invoked when the status of a network
     * stream's buffer has changed.
     *
     * @param listener the callback that will be run.
     */
    public void setOnBufferingUpdateListener(OnBufferingUpdateListener listener)
    {
        mOnBufferingUpdateListener = listener;
    }

    private OnBufferingUpdateListener mOnBufferingUpdateListener;

    /**
     * Interface definition of a callback to be invoked indicating
     * the completion of a seek operation.
     */
    public interface OnSeekCompleteListener
    {
        /**
         * Called to indicate the completion of a seek operation.
         *
         * @param mp the MediaPlayer that issued the seek operation
         */
        public void onSeekComplete(MediaPlayer mp);
    }

    /**
     * Register a callback to be invoked when a seek operation has been
     * completed.
     *
     * @param listener the callback that will be run
     */
    public void setOnSeekCompleteListener(OnSeekCompleteListener listener)
    {
        mOnSeekCompleteListener = listener;
    }

    private OnSeekCompleteListener mOnSeekCompleteListener;

    /**
     * Interface definition of a callback to be invoked when the
     * video size is first known or updated
     */
    public interface OnVideoSizeChangedListener
    {
        /**
         * Called to indicate the video size
         *
         * The video size (width and height) could be 0 if there was no video,
         * no display surface was set, or the value was not determined yet.
         *
         * @param mp        the MediaPlayer associated with this callback
         * @param width     the width of the video
         * @param height    the height of the video
         */
        public void onVideoSizeChanged(MediaPlayer mp, int width, int height);
    }

    /**
     * Register a callback to be invoked when the video size is
     * known or updated.
     *
     * @param listener the callback that will be run
     */
    public void setOnVideoSizeChangedListener(OnVideoSizeChangedListener listener)
    {
        mOnVideoSizeChangedListener = listener;
    }

    private OnVideoSizeChangedListener mOnVideoSizeChangedListener;

    /**
     * Interface definition of a callback to be invoked when the
     * speed is changed
     */
    public interface OnSpeedChangedListener
    {
        /**
         * Called to indicate the video speed is changed
         *
         * The video speed could be 1 if it was normal playback,
         *
         * @param mp        the MediaPlayer associated with this callback
         * @param speed     the new speed of the video
         */
        public void onSpeedChanged(MediaPlayer mp, int speed);
    }

    public void setOnSpeedChangedListener(OnSpeedChangedListener listener)
    {
        mOnSpeedChangedListener = listener;
    }
    
    private OnSpeedChangedListener mOnSpeedChangedListener;
    /**
     * Interface definition of a callback to be invoked when a
     * timed text is available for display.
     */
    public interface OnTimedTextListener
    {
        /**
         * Called to indicate an avaliable timed text
         *
         * @param mp             the MediaPlayer associated with this callback
         * @param text           the timed text sample which contains the text
         *                       needed to be displayed and the display format.
         */
        public void onTimedText(MediaPlayer mp, TimedText text);
    }

    /**
     * Register a callback to be invoked when a timed text is available
     * for display.
     *
     * @param listener the callback that will be run
     */
    public void setOnTimedTextListener(OnTimedTextListener listener)
    {
        mOnTimedTextListener = listener;
    }

    private OnTimedTextListener mOnTimedTextListener;

    /**
     * Interface definition of a callback to be invoked when a
     * track has data available.
     *
     * @hide
     */
    public interface OnSubtitleDataListener
    {
        public void onSubtitleData(MediaPlayer mp, SubtitleData data);
    }

    /**
     * Register a callback to be invoked when a track has data available.
     *
     * @param listener the callback that will be run
     *
     * @hide
     */
    public void setOnSubtitleDataListener(OnSubtitleDataListener listener)
    {
        mOnSubtitleDataListener = listener;
    }

    private OnSubtitleDataListener mOnSubtitleDataListener;

    /**
     * Interface definition of a callback to be invoked when a
     * track has timed metadata available.
     *
     * @see MediaPlayer#setOnTimedMetaDataAvailableListener(OnTimedMetaDataAvailableListener)
     */
    public interface OnTimedMetaDataAvailableListener
    {
        /**
         * Called to indicate avaliable timed metadata
         * <p>
         * This method will be called as timed metadata is extracted from the media,
         * in the same order as it occurs in the media. The timing of this event is
         * not controlled by the associated timestamp.
         *
         * @param mp             the MediaPlayer associated with this callback
         * @param data           the timed metadata sample associated with this event
         */
        public void onTimedMetaDataAvailable(MediaPlayer mp, TimedMetaData data);
    }

    /**
     * Register a callback to be invoked when a selected track has timed metadata available.
     * <p>
     * Currently only HTTP live streaming data URI's embedded with timed ID3 tags generates
     * {@link TimedMetaData}.
     *
     * @see MediaPlayer#selectTrack(int)
     * @see MediaPlayer.OnTimedMetaDataAvailableListener
     * @see TimedMetaData
     *
     * @param listener the callback that will be run
     */
    public void setOnTimedMetaDataAvailableListener(OnTimedMetaDataAvailableListener listener)
    {
        mOnTimedMetaDataAvailableListener = listener;
    }

    private OnTimedMetaDataAvailableListener mOnTimedMetaDataAvailableListener;

    /**
     * Interface definition of a callback to be invoked when the
     * seek in complete
     */
    public interface OnSeekResultListener
    {
        /**
         * Called to indicate the video seek is complete
         *
         * The result is 1 if the seek operation is success.
         *
         * @param mp        the MediaPlayer associated with this callback
         * @param result     the result of seek operation
         */
        public void onSeekResult(MediaPlayer mp, int result);
    }

    public void setOnSeekResultListener(OnSeekResultListener listener)
    {
        mOnSeekResultListener = listener;
    }

    private OnSeekResultListener mOnSeekResultListener;

    /* Do not change these values without updating their counterparts
     * in include/media/mediaplayer.h!
     */
    /** Unspecified media player error.
     * @see android.media.MediaPlayer.OnErrorListener
     */
    public static final int MEDIA_ERROR_UNKNOWN = 1;

    /** Media server died. In this case, the application must release the
     * MediaPlayer object and instantiate a new one.
     * @see android.media.MediaPlayer.OnErrorListener
     */
    public static final int MEDIA_ERROR_SERVER_DIED = 100;

    /** The video is streamed and its container is not valid for progressive
     * playback i.e the video's index (e.g moov atom) is not at the start of the
     * file.
     * @see android.media.MediaPlayer.OnErrorListener
     */
    public static final int MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK = 200;

    /** File or network related operation errors. */
    public static final int MEDIA_ERROR_IO = -1004;
    /** Bitstream is not conforming to the related coding standard or file spec. */
    public static final int MEDIA_ERROR_MALFORMED = -1007;
    /** Bitstream is conforming to the related coding standard or file spec, but
     * the media framework does not support the feature. */
    public static final int MEDIA_ERROR_UNSUPPORTED = -1010;
    /** Some operation takes too long to complete, usually more than 3-5 seconds. */
    public static final int MEDIA_ERROR_TIMED_OUT = -110;
    
    /** Video size exceed limitation */
    public static final int MEDIA_ERROR_VIDEO_SIZE_EXCEED_LIMITATION = -1000;
    /** Bitstream is not conforming to the related coding standard or file spec. */
    public static final int MEDIA_ERROR_BITSTREAM_ERROR = -1001;
    /* Fatal error with eror code */
    public static final int MEDIA_ERROR_FATAL = -1002;
    
    /* 
    *   Fatal error code 
    */
    public static final int FATALERR_GENERAL                    = 0x00000000;
    
    /* FOR AUDIO */
    public static final int FATALERR_AUDIO                      = 0x10000000;
    public static final int SPDIF_Error_Exceed_Threshold        = 0x10000001;
    public static final int AUDIO_DRY_OUT                       = 0x10000002;
    public static final int UNKNOWN_FORMAT                      = 0x10000003;
    public static final int UNRECOVERABLE_ERROR                 = 0x10000004;
    public static final int AUDIO_DATA_RECEIVE_ERROR            = 0x10000005;
    public static final int FATALERR_AUDIO_UNSUPPORTED_SAMPLERATE = 0x10000006;
    public static final int AUDIO_DIGITAL_OUT_ONLY              = 0x10000007;
    public static final int AUDIO_ASF_FILE_ERROR                = 0x10000008;
    public static final int FATALERR_AUDIO_PASSTHROUGH_ONLY     = 0x10000009;
    
    /* FOR VIDEO */
    public static final int FATALERR_VIDEO                      = 0x20000000;
    public static final int FATALERR_VIDEO_VBM_CONFIG           = 0x20000001;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_TRICK_PLAY  = 0x20000002;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNKNOWN_FRAME_RATE      = 0x20000003;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNKNOWN_START_CODE      = 0x20000004;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNKNOWN_PICTURE_CODING_TYPE = 0x20000005;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNKNOWN_PICTURE_STRUCTURE   = 0x20000006;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNKNOWN_EXTENSION           = 0x20000007;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_RESOLUTION      = 0x20000008;
    public static final int FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_EXTENSION       = 0x20000009;
    public static final int FATALERR_VIDEO_MPEG2DEC_INVALID_MARKER_BIT          = 0x2000000A;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_GMC             = 0x2000000B;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNKNOWN_START_CODE          = 0x2000000C;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNKNOWN_FRAME_RATE_CODE     = 0x2000000D;
    public static final int FATALERR_VIDEO_MPEG4DEC_WRONG_PIC_TYPE              = 0x2000000E;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_NOT_RECTANGULAR = 0x2000000F;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_SPRITE          = 0x20000010;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_NOT8BIT         = 0x20000011;
    public static final int FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_SCALABILITY     = 0x20000012;
    public static final int FATALERR_VIDEO_H264DEC_UNSUPPORTED_PROFILE          = 0x20000013;
    public static final int FATALERR_VIDEO_ALLOC_COMEM                          = 0x20000014;
    /* FOR SYSTEM */
    public static final int FATALERR_SYSTEM                                     = 0x30000000;
    public static final int FATALERR_SYSTEM_TRANSCODE_SERVER_ERROR              = 0x30000001;
    public static final int FATALERR_SYSTEM_MEDIA_REMOVED                       = 0x30000002;
    public static final int FATALERR_SYSTEM_EIO                                 = 0x30000003;


    /** Unspecified low-level system error. This value originated from UNKNOWN_ERROR in
     * system/core/include/utils/Errors.h
     * @see android.media.MediaPlayer.OnErrorListener
     * @hide
     */
    public static final int MEDIA_ERROR_SYSTEM = -2147483648;

    /**
     * Interface definition of a callback to be invoked when there
     * has been an error during an asynchronous operation (other errors
     * will throw exceptions at method call time).
     */
    public interface OnErrorListener
    {
        /**
         * Called to indicate an error.
         *
         * @param mp      the MediaPlayer the error pertains to
         * @param what    the type of error that has occurred:
         * <ul>
         * <li>{@link #MEDIA_ERROR_UNKNOWN}
         * <li>{@link #MEDIA_ERROR_SERVER_DIED}
         * </ul>
         * @param extra an extra code, specific to the error. Typically
         * implementation dependent.
         * <ul>
         * <li>{@link #MEDIA_ERROR_IO}
         * <li>{@link #MEDIA_ERROR_MALFORMED}
         * <li>{@link #MEDIA_ERROR_UNSUPPORTED}
         * <li>{@link #MEDIA_ERROR_TIMED_OUT}
         * <li><code>MEDIA_ERROR_SYSTEM (-2147483648)</code> - low-level system error.
         * </ul>
         * @return True if the method handled the error, false if it didn't.
         * Returning false, or not having an OnErrorListener at all, will
         * cause the OnCompletionListener to be called.
         */
        boolean onError(MediaPlayer mp, int what, int extra);
    }

    /**
     * Register a callback to be invoked when an error has happened
     * during an asynchronous operation.
     *
     * @param listener the callback that will be run
     */
    public void setOnErrorListener(OnErrorListener listener)
    {
        mOnErrorListener = listener;
    }

    private OnErrorListener mOnErrorListener;


    /* Do not change these values without updating their counterparts
     * in include/media/mediaplayer.h!
     */
    /** Unspecified media player info.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_UNKNOWN = 1;

    /** The player was started because it was used as the next player for another
     * player, which just completed playback.
     * @see android.media.MediaPlayer.OnInfoListener
     * @hide
     */
    public static final int MEDIA_INFO_STARTED_AS_NEXT = 2;

    /** The player just pushed the very first video frame for rendering.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_VIDEO_RENDERING_START = 3;

    /** The video is too complex for the decoder: it can't decode frames fast
     *  enough. Possibly only the audio plays fine at this stage.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_VIDEO_TRACK_LAGGING = 700;

    /** MediaPlayer is temporarily pausing playback internally in order to
     * buffer more data.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_BUFFERING_START = 701;

    /** MediaPlayer is resuming playback after filling buffers.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_BUFFERING_END = 702;

    /** Estimated network bandwidth information (kbps) is available; currently this event fires
     * simultaneously as {@link #MEDIA_INFO_BUFFERING_START} and {@link #MEDIA_INFO_BUFFERING_END}
     * when playing network files.
     * @see android.media.MediaPlayer.OnInfoListener
     * @hide
     */
    public static final int MEDIA_INFO_NETWORK_BANDWIDTH = 703;

    /** Bad interleaving means that a media has been improperly interleaved or
     * not interleaved at all, e.g has all the video samples first then all the
     * audio ones. Video is playing but a lot of disk seeks may be happening.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_BAD_INTERLEAVING = 800;

    /** The media cannot be seeked (e.g live stream)
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_NOT_SEEKABLE = 801;

    /** A new set of metadata is available.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_METADATA_UPDATE = 802;

    /** A new set of external-only metadata is available.  Used by
     *  JAVA framework to avoid triggering track scanning.
     * @hide
     */
    public static final int MEDIA_INFO_EXTERNAL_METADATA_UPDATE = 803;

    /** The DVD domain is changed
     * @hide
     */
    public static final int MEDIA_INFO_DOMAIN_UPDATE = 804;

    /** The DVD title is changed
     * @hide
     */
    public static final int MEDIA_INFO_TITLE_UPDATE = 805;

    /** The DVD chapter is changed
     * @hide
     */
    public static final int MEDIA_INFO_CHAPTER_UPDATE = 806;

    /** Audio occurs fatal error
     * @hide
     */
    public static final int MEDIA_INFO_AUDIO_FATAL_ERROR = 807;

    /** Failed to handle timed text track properly.
     * @see android.media.MediaPlayer.OnInfoListener
     *
     * {@hide}
     */
    public static final int MEDIA_INFO_TIMED_TEXT_ERROR = 900;

    /** Subtitle track was not supported by the media framework.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_UNSUPPORTED_SUBTITLE = 901;

    /** Reading the subtitle track takes too long.
     * @see android.media.MediaPlayer.OnInfoListener
     */
    public static final int MEDIA_INFO_SUBTITLE_TIMED_OUT = 902;

    /**
     * Interface definition of a callback to be invoked to communicate some
     * info and/or warning about the media or its playback.
     */
    public interface OnInfoListener
    {
        /**
         * Called to indicate an info or a warning.
         *
         * @param mp      the MediaPlayer the info pertains to.
         * @param what    the type of info or warning.
         * <ul>
         * <li>{@link #MEDIA_INFO_UNKNOWN}
         * <li>{@link #MEDIA_INFO_VIDEO_TRACK_LAGGING}
         * <li>{@link #MEDIA_INFO_VIDEO_RENDERING_START}
         * <li>{@link #MEDIA_INFO_BUFFERING_START}
         * <li>{@link #MEDIA_INFO_BUFFERING_END}
         * <li><code>MEDIA_INFO_NETWORK_BANDWIDTH (703)</code> -
         *     bandwidth information is available (as <code>extra</code> kbps)
         * <li>{@link #MEDIA_INFO_BAD_INTERLEAVING}
         * <li>{@link #MEDIA_INFO_NOT_SEEKABLE}
         * <li>{@link #MEDIA_INFO_METADATA_UPDATE}
         * <li>{@link #MEDIA_INFO_UNSUPPORTED_SUBTITLE}
         * <li>{@link #MEDIA_INFO_SUBTITLE_TIMED_OUT}
         * </ul>
         * @param extra an extra code, specific to the info. Typically
         * implementation dependent.
         * @return True if the method handled the info, false if it didn't.
         * Returning false, or not having an OnInfoListener at all, will
         * cause the info to be discarded.
         */
        boolean onInfo(MediaPlayer mp, int what, int extra);
    }

    /**
     * Register a callback to be invoked when an info/warning is available.
     *
     * @param listener the callback that will be run
     */
    public void setOnInfoListener(OnInfoListener listener)
    {
        mOnInfoListener = listener;
    }

    private OnInfoListener mOnInfoListener;

    /*
     * Test whether a given video scaling mode is supported.
     */
    private boolean isVideoScalingModeSupported(int mode) {
        return (mode == VIDEO_SCALING_MODE_SCALE_TO_FIT ||
                mode == VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
    }

// added by RTK
        private volatile SpeedMeasureThread measure = null;
        private int kbytesPerSecond = 0;
        private class SpeedMeasureThread extends Thread{
            private static final String TAG="SpeedMeasureThread";
            @Override
            public void run() {
                Log.d(TAG, "SpeedMeasureThread executed");

                long startBytes = TrafficStats.getTotalRxBytes(); /*gets total bytes received so far*/
                long startTime = System.currentTimeMillis();

                long endTime;
                long endBytes; /*gets total bytes received so far*/
                long totalSeconds;
                long totalKB = 0;

                while(true){
                    try {
                        Thread.sleep(2000);
                        endTime = System.currentTimeMillis();
                        endBytes = TrafficStats.getTotalRxBytes(); /*gets total bytes received so far*/
                        totalSeconds = (endTime - startTime)/1000;
                        totalKB = (endBytes - startBytes)>>10;
                        //Log.d(TAG, "totalSeconds = " + totalSeconds + ", totalKilobyte = " + totalBytes);

                        kbytesPerSecond = (int) (totalKB/totalSeconds);
                        //Log.d(TAG, "bytesPerSecond = " + bytesPerSecond + " KB/s");

                        Message msg = Message.obtain(mEventHandler, MEDIA_INFO, MEDIA_INFO_NETWORK_BANDWIDTH, kbytesPerSecond);
                        Log.d(TAG, "sending Message:" + msg);
                        mEventHandler.sendMessage(msg);
                    } catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        //e.printStackTrace();
                        Log.d(TAG, "SpeedMeasureThread is interrupted");
                        break;
                    }
                }
            }
        }
// end of added by RTK

    /** @hide */
    static class TimeProvider implements MediaPlayer.OnSeekCompleteListener,
            MediaTimeProvider {
        private static final String TAG = "MTP";
        private static final long MAX_NS_WITHOUT_POSITION_CHECK = 5000000000L;
        private static final long MAX_EARLY_CALLBACK_US = 1000;
        private static final long TIME_ADJUSTMENT_RATE = 2;  /* meaning 1/2 */
        private long mLastTimeUs = 0;
        private MediaPlayer mPlayer;
        private boolean mPaused = true;
        private boolean mStopped = true;
        private boolean mBuffering;
        private long mLastReportedTime;
        private long mTimeAdjustment;
        // since we are expecting only a handful listeners per stream, there is
        // no need for log(N) search performance
        private MediaTimeProvider.OnMediaTimeListener mListeners[];
        private long mTimes[];
        private long mLastNanoTime;
        private Handler mEventHandler;
        private boolean mRefresh = false;
        private boolean mPausing = false;
        private boolean mSeeking = false;
        private static final int NOTIFY = 1;
        private static final int NOTIFY_TIME = 0;
        private static final int REFRESH_AND_NOTIFY_TIME = 1;
        private static final int NOTIFY_STOP = 2;
        private static final int NOTIFY_SEEK = 3;
        private static final int NOTIFY_TRACK_DATA = 4;
        private HandlerThread mHandlerThread;

        /** @hide */
        public boolean DEBUG = false;

        public TimeProvider(MediaPlayer mp) {
            mPlayer = mp;
            try {
                getCurrentTimeUs(true, false);
            } catch (IllegalStateException e) {
                // we assume starting position
                mRefresh = true;
            }

            Looper looper;
            if ((looper = Looper.myLooper()) == null &&
                (looper = Looper.getMainLooper()) == null) {
                // Create our own looper here in case MP was created without one
                mHandlerThread = new HandlerThread("MediaPlayerMTPEventThread",
                      Process.THREAD_PRIORITY_FOREGROUND);
                mHandlerThread.start();
                looper = mHandlerThread.getLooper();
            }
            mEventHandler = new EventHandler(looper);

            mListeners = new MediaTimeProvider.OnMediaTimeListener[0];
            mTimes = new long[0];
            mLastTimeUs = 0;
            mTimeAdjustment = 0;
        }

        private void scheduleNotification(int type, long delayUs) {
            // ignore time notifications until seek is handled
            if (mSeeking &&
                    (type == NOTIFY_TIME || type == REFRESH_AND_NOTIFY_TIME)) {
                return;
            }

            if (DEBUG) Log.v(TAG, "scheduleNotification " + type + " in " + delayUs);
            mEventHandler.removeMessages(NOTIFY);
            Message msg = mEventHandler.obtainMessage(NOTIFY, type, 0);
            mEventHandler.sendMessageDelayed(msg, (int) (delayUs / 1000));
        }

        /** @hide */
        public void close() {
            mEventHandler.removeMessages(NOTIFY);
            if (mHandlerThread != null) {
                mHandlerThread.quitSafely();
                mHandlerThread = null;
            }
        }

        /** @hide */
        protected void finalize() {
            if (mHandlerThread != null) {
                mHandlerThread.quitSafely();
            }
        }

        /** @hide */
        public void onPaused(boolean paused) {
            synchronized(this) {
                if (DEBUG) Log.d(TAG, "onPaused: " + paused);
                if (mStopped) { // handle as seek if we were stopped
                    mStopped = false;
                    mSeeking = true;
                    scheduleNotification(NOTIFY_SEEK, 0 /* delay */);
                } else {
                    mPausing = paused;  // special handling if player disappeared
                    mSeeking = false;
                    scheduleNotification(REFRESH_AND_NOTIFY_TIME, 0 /* delay */);
                }
            }
        }

        /** @hide */
        public void onBuffering(boolean buffering) {
            synchronized (this) {
                if (DEBUG) Log.d(TAG, "onBuffering: " + buffering);
                mBuffering = buffering;
                scheduleNotification(REFRESH_AND_NOTIFY_TIME, 0 /* delay */);
            }
        }

        /** @hide */
        public void onStopped() {
            synchronized(this) {
                if (DEBUG) Log.d(TAG, "onStopped");
                mPaused = true;
                mStopped = true;
                mSeeking = false;
                mBuffering = false;
                scheduleNotification(NOTIFY_STOP, 0 /* delay */);
            }
        }

        /** @hide */
        @Override
        public void onSeekComplete(MediaPlayer mp) {
            synchronized(this) {
                mStopped = false;
                mSeeking = true;
                scheduleNotification(NOTIFY_SEEK, 0 /* delay */);
            }
        }

        /** @hide */
        public void onNewPlayer() {
            if (mRefresh) {
                synchronized(this) {
                    mStopped = false;
                    mSeeking = true;
                    mBuffering = false;
                    scheduleNotification(NOTIFY_SEEK, 0 /* delay */);
                }
            }
        }

        private synchronized void notifySeek() {
            mSeeking = false;
            try {
                long timeUs = getCurrentTimeUs(true, false);
                if (DEBUG) Log.d(TAG, "onSeekComplete at " + timeUs);

                for (MediaTimeProvider.OnMediaTimeListener listener: mListeners) {
                    if (listener == null) {
                        break;
                    }
                    listener.onSeek(timeUs);
                }
            } catch (IllegalStateException e) {
                // we should not be there, but at least signal pause
                if (DEBUG) Log.d(TAG, "onSeekComplete but no player");
                mPausing = true;  // special handling if player disappeared
                notifyTimedEvent(false /* refreshTime */);
            }
        }

        private synchronized void notifyTrackData(Pair<SubtitleTrack, byte[]> trackData) {
            SubtitleTrack track = trackData.first;
            byte[] data = trackData.second;
            track.onData(data, true /* eos */, ~0 /* runID: keep forever */);
        }

        private synchronized void notifyStop() {
            for (MediaTimeProvider.OnMediaTimeListener listener: mListeners) {
                if (listener == null) {
                    break;
                }
                listener.onStop();
            }
        }

        private int registerListener(MediaTimeProvider.OnMediaTimeListener listener) {
            int i = 0;
            for (; i < mListeners.length; i++) {
                if (mListeners[i] == listener || mListeners[i] == null) {
                    break;
                }
            }

            // new listener
            if (i >= mListeners.length) {
                MediaTimeProvider.OnMediaTimeListener[] newListeners =
                    new MediaTimeProvider.OnMediaTimeListener[i + 1];
                long[] newTimes = new long[i + 1];
                System.arraycopy(mListeners, 0, newListeners, 0, mListeners.length);
                System.arraycopy(mTimes, 0, newTimes, 0, mTimes.length);
                mListeners = newListeners;
                mTimes = newTimes;
            }

            if (mListeners[i] == null) {
                mListeners[i] = listener;
                mTimes[i] = MediaTimeProvider.NO_TIME;
            }
            return i;
        }

        public void notifyAt(
                long timeUs, MediaTimeProvider.OnMediaTimeListener listener) {
            synchronized(this) {
                if (DEBUG) Log.d(TAG, "notifyAt " + timeUs);
                mTimes[registerListener(listener)] = timeUs;
                scheduleNotification(NOTIFY_TIME, 0 /* delay */);
            }
        }

        public void scheduleUpdate(MediaTimeProvider.OnMediaTimeListener listener) {
            synchronized(this) {
                if (DEBUG) Log.d(TAG, "scheduleUpdate");
                int i = registerListener(listener);

                if (!mStopped) {
                    mTimes[i] = 0;
                    scheduleNotification(NOTIFY_TIME, 0 /* delay */);
                }
            }
        }

        public void cancelNotifications(
                MediaTimeProvider.OnMediaTimeListener listener) {
            synchronized(this) {
                int i = 0;
                for (; i < mListeners.length; i++) {
                    if (mListeners[i] == listener) {
                        System.arraycopy(mListeners, i + 1,
                                mListeners, i, mListeners.length - i - 1);
                        System.arraycopy(mTimes, i + 1,
                                mTimes, i, mTimes.length - i - 1);
                        mListeners[mListeners.length - 1] = null;
                        mTimes[mTimes.length - 1] = NO_TIME;
                        break;
                    } else if (mListeners[i] == null) {
                        break;
                    }
                }

                scheduleNotification(NOTIFY_TIME, 0 /* delay */);
            }
        }

        private synchronized void notifyTimedEvent(boolean refreshTime) {
            // figure out next callback
            long nowUs;
            try {
                nowUs = getCurrentTimeUs(refreshTime, true);
            } catch (IllegalStateException e) {
                // assume we paused until new player arrives
                mRefresh = true;
                mPausing = true; // this ensures that call succeeds
                nowUs = getCurrentTimeUs(refreshTime, true);
            }
            long nextTimeUs = nowUs;

            if (mSeeking) {
                // skip timed-event notifications until seek is complete
                return;
            }

            if (DEBUG) {
                StringBuilder sb = new StringBuilder();
                sb.append("notifyTimedEvent(").append(mLastTimeUs).append(" -> ")
                        .append(nowUs).append(") from {");
                boolean first = true;
                for (long time: mTimes) {
                    if (time == NO_TIME) {
                        continue;
                    }
                    if (!first) sb.append(", ");
                    sb.append(time);
                    first = false;
                }
                sb.append("}");
                Log.d(TAG, sb.toString());
            }

            Vector<MediaTimeProvider.OnMediaTimeListener> activatedListeners =
                new Vector<MediaTimeProvider.OnMediaTimeListener>();
            for (int ix = 0; ix < mTimes.length; ix++) {
                if (mListeners[ix] == null) {
                    break;
                }
                if (mTimes[ix] <= NO_TIME) {
                    // ignore, unless we were stopped
                } else if (mTimes[ix] <= nowUs + MAX_EARLY_CALLBACK_US) {
                    activatedListeners.add(mListeners[ix]);
                    if (DEBUG) Log.d(TAG, "removed");
                    mTimes[ix] = NO_TIME;
                } else if (nextTimeUs == nowUs || mTimes[ix] < nextTimeUs) {
                    nextTimeUs = mTimes[ix];
                }
            }

            if (nextTimeUs > nowUs && !mPaused) {
                // schedule callback at nextTimeUs
                if (DEBUG) Log.d(TAG, "scheduling for " + nextTimeUs + " and " + nowUs);
                scheduleNotification(NOTIFY_TIME, nextTimeUs - nowUs);
            } else {
                mEventHandler.removeMessages(NOTIFY);
                // no more callbacks
            }

            for (MediaTimeProvider.OnMediaTimeListener listener: activatedListeners) {
                listener.onTimedEvent(nowUs);
            }
        }

        private long getEstimatedTime(long nanoTime, boolean monotonic) {
            if (mPaused) {
                mLastReportedTime = mLastTimeUs + mTimeAdjustment;
            } else {
                long timeSinceRead = (nanoTime - mLastNanoTime) / 1000;
                mLastReportedTime = mLastTimeUs + timeSinceRead;
                if (mTimeAdjustment > 0) {
                    long adjustment =
                        mTimeAdjustment - timeSinceRead / TIME_ADJUSTMENT_RATE;
                    if (adjustment <= 0) {
                        mTimeAdjustment = 0;
                    } else {
                        mLastReportedTime += adjustment;
                    }
                }
            }
            return mLastReportedTime;
        }

        public long getCurrentTimeUs(boolean refreshTime, boolean monotonic)
                throws IllegalStateException {
            synchronized (this) {
                // we always refresh the time when the paused-state changes, because
                // we expect to have received the pause-change event delayed.
                if (mPaused && !refreshTime) {
                    return mLastReportedTime;
                }

                long nanoTime = System.nanoTime();
                if (refreshTime ||
                        nanoTime >= mLastNanoTime + MAX_NS_WITHOUT_POSITION_CHECK || mLastTimeUs<=MAX_NS_WITHOUT_POSITION_CHECK) {
                    try {
                        mLastTimeUs = mPlayer.getCurrentPosition() * 1000L;
                        mPaused = !mPlayer.isPlaying() || mBuffering;
                        if (DEBUG) Log.v(TAG, (mPaused ? "paused" : "playing") + " at " + mLastTimeUs);
                    } catch (IllegalStateException e) {
                        if (mPausing) {
                            // if we were pausing, get last estimated timestamp
                            mPausing = false;
                            getEstimatedTime(nanoTime, monotonic);
                            mPaused = true;
                            if (DEBUG) Log.d(TAG, "illegal state, but pausing: estimating at " + mLastReportedTime);
                            return mLastReportedTime;
                        }
                        // TODO get time when prepared
                        throw e;
                    }
                    mLastNanoTime = nanoTime;
                    if (monotonic && mLastTimeUs < mLastReportedTime) {
                        /* have to adjust time */
                        mTimeAdjustment = mLastReportedTime - mLastTimeUs;
                        if (mTimeAdjustment > 1000000) {
                            // schedule seeked event if time jumped significantly
                            // TODO: do this properly by introducing an exception
                            mStopped = false;
                            mSeeking = true;
                            scheduleNotification(NOTIFY_SEEK, 0 /* delay */);
                        }
                    } else {
                        mTimeAdjustment = 0;
                    }
                }

                return getEstimatedTime(nanoTime, monotonic);
            }
        }

        private class EventHandler extends Handler {
            public EventHandler(Looper looper) {
                super(looper);
            }

            @Override
            public void handleMessage(Message msg) {
                if (msg.what == NOTIFY) {
                    switch (msg.arg1) {
                    case NOTIFY_TIME:
                        notifyTimedEvent(false /* refreshTime */);
                        break;
                    case REFRESH_AND_NOTIFY_TIME:
                        notifyTimedEvent(true /* refreshTime */);
                        break;
                    case NOTIFY_STOP:
                        notifyStop();
                        break;
                    case NOTIFY_SEEK:
                        notifySeek();
                        break;
                    case NOTIFY_TRACK_DATA:
                        notifyTrackData((Pair<SubtitleTrack, byte[]>)msg.obj);
                        break;
                    }
                }
            }
        }
    }
}
