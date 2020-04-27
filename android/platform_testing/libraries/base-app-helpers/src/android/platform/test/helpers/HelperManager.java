/*
 * Copyright (C) 2016 The Android Open Source Project
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

package android.platform.test.helpers;

import android.app.Instrumentation;
import android.content.Context;
import android.util.Log;
import dalvik.system.DexFile;
import java.io.File;
import java.io.IOException;
import java.lang.ClassLoader;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;
import java.lang.NoSuchMethodException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;

/**
 * The HelperManager class is used to remove any explicit or hard-coded dependencies on app helper
 * implementations. Instead, it provides a method for abstracting helper instantiation by specifying
 * a base class for which you require an implementation. The manager effectively searches for a
 * suitable implementation using runtime class loading.
 * <p>
 * The class provides two means for finding the necessary classes:
 * <ol>
 * <li> Static inclusion - if all the code is included in the final APK, the Context can be used to
 * generate a HelperManager and to instantiate implementations.
 * <li> Dexed file inclusion - if this and the helper implementations are bundled into a dexed
 * file, such as a dexed JAR, that gets added after compilation, e.g. how the automated user
 * profile test suites are built, the path can be used to generate a HelperManager and to
 * instantiate implementations.
 * </ol>
 * <p>
 * Including and using this strategy will prune the explicit dependency tree for the App Helper
 * Library and provide a more robust library for use across the Android source tree.
 */
public class HelperManager {
    private static final String LOG_TAG = HelperManager.class.getSimpleName();
    private static HelperManager sInstance;

    /**
     * Returns an instance of the HelperManager that searches the supplied application context for
     * classes to instantiate to helper implementations.
     *
     * @param context the application context to search
     * @param instr the active instrumentation
     * @return a new instance of the HelperManager class
     */
    public static HelperManager getInstance(Context context, Instrumentation instr) {
        if (sInstance == null) {
            // Input checks
            if (context == null) {
                throw new NullPointerException("Cannot pass in a null context.");
            }
            if (instr == null) {
                throw new NullPointerException(
                        String.format("Cannot pass in null instrumentation."));
            }
            // Instantiation
            sInstance = new HelperManager(context.getPackageCodePath(), instr);
        }
        return sInstance;
    }

    /**
     * Returns an instance of the HelperManager that searches the supplied location for classes to
     * instantiate to helper implementations.
     *
     * @param path the dexed file where the classes are included
     * @param instr the active instrumentation
     * @throws IllegalArgumentException if the path is not a valid file
     * @return a new instance of the HelperManager class
     */
    public static HelperManager getInstance(String path, Instrumentation instr) {
        if (sInstance == null) {
            // Input checks
            if (!(new File(path)).exists()) {
                throw new IllegalArgumentException(
                        String.format("No file found at path: %s.", path));
            }
            if (instr == null) {
                throw new NullPointerException(
                        String.format("Cannot pass in null instrumentation."));
            }
            // Instantiation
            sInstance = new HelperManager(path, instr);
        }
        return sInstance;
    }

    private Instrumentation mInstrumentation;
    private List<String> mClasses;

    private HelperManager(String path, Instrumentation instr) {
        mInstrumentation = instr;
        // Collect all of the available classes
        try {
            DexFile dex = new DexFile(path);
            mClasses = Collections.list(dex.entries());
        } catch (IOException e) {
            throw new RuntimeException("Failed to retrieve the dex file.");
        }
    }

    /**
     * Returns a concrete implementation of the helper interface supplied, if available.
     *
     * @param base the interface base class to find an implementation for
     * @throws RuntimeException if no implementation is found
     * @return a concrete implementation of base
     */
    public <T extends AbstractStandardAppHelper> T get(Class<T> base) {
        ClassLoader loader = HelperManager.class.getClassLoader();
        // Iterate and search for the implementation
        for (String className : mClasses) {
            Class<?> clazz = null;
            try {
                clazz = loader.loadClass(className);
            } catch (ClassNotFoundException e) {
                Log.w(LOG_TAG, String.format("Class not found: %s", className));
            }
            if (base.isAssignableFrom(clazz) && !clazz.equals(base)) {
                // Instantiate the implementation class and return
                try {
                    Constructor<?> constructor = clazz.getConstructor(Instrumentation.class);
                    return (T)constructor.newInstance(mInstrumentation);
                } catch (NoSuchMethodException e) {
                    Log.w(LOG_TAG, String.format("Failed to find a matching constructor for %s",
                            className), e);
                } catch (IllegalAccessException e) {
                    Log.w(LOG_TAG, String.format("Failed to access the constructor %s",
                            className), e);
                } catch (InstantiationException e) {
                    Log.w(LOG_TAG, String.format("Failed to instantiate %s",
                            className), e);
                } catch (InvocationTargetException e) {
                    Log.w(LOG_TAG, String.format("Exception encountered instantiating %s",
                            className), e);
                }
            }
        }
        throw new RuntimeException(
                String.format("Failed to find an implementation for %s", base.toString()));
    }
}
