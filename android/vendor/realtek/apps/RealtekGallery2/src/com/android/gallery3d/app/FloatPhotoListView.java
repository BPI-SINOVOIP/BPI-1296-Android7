/*
 * Copyright (C) 2010 The Android Open Source Project
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

import android.content.Context;
import android.graphics.Matrix;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Transformation;
import android.widget.Gallery;
import android.widget.SpinnerAdapter;
import android.widget.ImageView;

public class FloatPhotoListView extends Gallery
{
    // =============================================================================
    // Private members
    // =============================================================================

    private float unselectedAlpha = 0.5f;
    private float unselectedScale = 1.0f;

    // =============================================================================
    // Constructors
    // =============================================================================

    public FloatPhotoListView(Context context)
    {
        super(context);
        this.initialize();
    }

    public FloatPhotoListView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        this.initialize();
    }

    public FloatPhotoListView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        this.initialize();
    }

    private void initialize()
    {
        this.setSpacing(0);
    }

    /**
    * Use this to provide a {@link FloatPhotoAdapter} to the FloatPhotoListView. This
    * method will throw an {@link ClassCastException} if the passed adapter does not
    * subclass {@link FloatPhotoAdapter}.
    *
    * @param adapter
    */
    @Override
    public void setAdapter(SpinnerAdapter adapter)
    {
        if (!(adapter instanceof FloatPhotoAdapter))
        {
            throw new ClassCastException(FloatPhotoListView.class.getSimpleName() + " only works in conjunction with a " + FloatPhotoAdapter.class.getSimpleName());
        }
        super.setAdapter(adapter);
    }

    public float getUnselectedAlpha()
    {
        return this.unselectedAlpha;
    }

    @Override
    public void setUnselectedAlpha(float unselectedAlpha)
    {
        super.setUnselectedAlpha(unselectedAlpha);
        this.unselectedAlpha = unselectedAlpha;
    }

    @Override
    protected boolean getChildStaticTransformation(View child, Transformation trans)
    {
        ImageView item = (ImageView) child;
        item.invalidate();

        final int floatPhotoWidth = this.getWidth();
        final int floatPhotoCenter = floatPhotoWidth / 2;
        final int childWidth = item.getWidth();
        final int childCenter = item.getLeft() + childWidth / 2;
        final int actionDistance = (int) ((floatPhotoWidth + childWidth) / 2.0f);
        // Calculate the abstract amount for all effects.
        final float effectsAmount = Math.min(1.0f, Math.max(-1.0f, (1.0f / actionDistance) * (childCenter - floatPhotoCenter)));

        // Clear previous transformations and set transformation type (matrix + alpha).
        trans.clear();
        trans.setTransformationType(Transformation.TYPE_BOTH);

        // Alpha
        if (this.unselectedAlpha != 1) {
            final float alphaAmount = (this.unselectedAlpha - 1) * Math.abs(effectsAmount) + 1;
            trans.setAlpha(alphaAmount);
        }
        return true;
    }

    // =============================================================================
    // Public classes
    // =============================================================================

    public static class LayoutParams extends Gallery.LayoutParams
    {
        public LayoutParams(Context c, AttributeSet attrs)
        {
            super(c, attrs);
        }

        public LayoutParams(int w, int h)
        {
            super(w, h);
        }

        public LayoutParams(ViewGroup.LayoutParams source)
        {
            super(source);
        }
    }
}
