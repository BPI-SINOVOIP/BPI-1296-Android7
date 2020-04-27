/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft, in password field display "*" instead of ".".
 */

package com.realtek.realtekinitialsettings;

import android.view.View;
import android.text.method.PasswordTransformationMethod;

public class AsteriskPasswordTransformationMethod extends PasswordTransformationMethod {
    @Override
    public CharSequence getTransformation(CharSequence source, View view) {
        return new PasswordCharSequence(source);
    }

    private class PasswordCharSequence implements CharSequence {
        private CharSequence mSource;
        public PasswordCharSequence(CharSequence source) {
            mSource = source; // Store char sequence
        }
        public char charAt(int index) {
            return '*'; // This is the important part
        }
        public int length() {
            return mSource.length(); // Return default
        }
        public CharSequence subSequence(int start, int end) {
            return mSource.subSequence(start, end); // Return default
        }
    }
};
