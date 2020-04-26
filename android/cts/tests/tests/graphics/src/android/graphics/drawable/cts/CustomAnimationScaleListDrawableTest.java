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

package android.graphics.drawable.cts;

import android.animation.ValueAnimator;
import android.graphics.cts.R;
import android.graphics.drawable.Animatable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.DrawableContainer;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.MediumTest;

/**
 * This test is used to verify that the CustomAnimationScaleListDrawable's current drawable depends
 * on animation duration scale. When the scale is 0, it is a static drawable, otherwise, it is an
 * animatable drawable.
 */
public class CustomAnimationScaleListDrawableTest extends AndroidTestCase {
    @MediumTest
    public void testNonZeroDurationScale() {
        float originalScale = ValueAnimator.getDurationScale();
        ValueAnimator.setDurationScale(2.0f);
        Drawable dr = getContext().getDrawable(R.drawable.custom_animation_scale_list_drawable);
        assertTrue(dr instanceof DrawableContainer);

        assertTrue(dr.getCurrent() instanceof Animatable);
        ValueAnimator.setDurationScale(originalScale);
    }

    @MediumTest
    public void testZeroDurationScale() {
        float originalScale = ValueAnimator.getDurationScale();
        ValueAnimator.setDurationScale(0f);
        Drawable dr = getContext().getDrawable(R.drawable.custom_animation_scale_list_drawable);
        assertTrue(dr instanceof DrawableContainer);
        assertFalse(dr.getCurrent() instanceof Animatable);
        ValueAnimator.setDurationScale(originalScale);
    }
}
