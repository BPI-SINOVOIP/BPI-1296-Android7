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
package com.android.quicksearchbox;

import android.content.ComponentName;

import android.util.Log;


/**
 * A Suggestion that delegates all calls to other suggestions.
 */
public abstract class AbstractSuggestionWrapper implements Suggestion {

    /**
     * Gets the current suggestion.
     */
    protected abstract Suggestion current();

    public String getCorpusTitle(){return null;}

    public String getShortcutId() {
        if (current() != null)
            return current().getShortcutId();
        else
            return null;
    }

    public String getSuggestionFormat() {
        if (current() != null)
            return current().getSuggestionFormat();
        else
            return null; 
    }

    public String getSuggestionIcon1() {
        
        
        if (current() != null)
        {
            return current().getSuggestionIcon1();
        }
        else
        {
            return null;
        }
    }

    public String getSuggestionIcon2() {
        if (current() != null)
            return current().getSuggestionIcon2();
        else
            return null;
    }

    public String getSuggestionIntentAction() {
        if (current() != null)
            return current().getSuggestionIntentAction();
        else
            return null;
    }

    public ComponentName getSuggestionIntentComponent() {
        if (current() != null)
            return current().getSuggestionIntentComponent();
        else
            return null;
    }

    public String getSuggestionIntentDataString() {
        if (current() != null)
            return current().getSuggestionIntentDataString();
        else
            return null;
    }

    public String getSuggestionIntentExtraData() {
        if (current() != null)
            return current().getSuggestionIntentExtraData();
        else
            return null;
    }

    public String getSuggestionLogType() {
        if (current() != null)
            return current().getSuggestionLogType();
        else
            return null;
    }

    public String getSuggestionQuery() {
        if (current() != null)
            return current().getSuggestionQuery();
        else
            return getCorpusTitle();
    }

    public Source getSuggestionSource() {
        if (current() != null)
            return current().getSuggestionSource();
        else
            return null;
    }

    public String getSuggestionText1() {
        if (current() != null)
            return current().getSuggestionText1();
        else
            return getCorpusTitle();
    }

    public String getSuggestionText2() {
        if (current() != null)
            return current().getSuggestionText2();
        else
            return null;
    }

    public String getSuggestionText2Url() {
        if (current() != null)
            return current().getSuggestionText2Url();
        else
            return null;
    }

    public boolean isSpinnerWhileRefreshing() {
        if (current() != null)
            return current().isSpinnerWhileRefreshing();
        else
            return false;
    }

    public boolean isSuggestionShortcut() {
        if (current() != null)
            return current().isSuggestionShortcut();
        else
            return false;
    }

    public boolean isWebSearchSuggestion() {
        if (current() != null)
            return current().isWebSearchSuggestion();
        else
            return false;
    }

    public boolean isHistorySuggestion() {
        if (current() != null)
            return current().isHistorySuggestion();
        else
            return false;
    }

    public SuggestionExtras getExtras() {
        if (current() != null)
            return current().getExtras();
        else
            return null;
    }

}
