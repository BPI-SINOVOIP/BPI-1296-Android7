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
package com.android.quicksearchbox.ui;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;

/**
 * Suggestions view that displays suggestions clustered by corpus type.
 */
public class ClusteredSuggestionsView extends ExpandableListView
        implements SuggestionsListView<ExpandableListAdapter> {

    SuggestionsAdapter<ExpandableListAdapter> mSuggestionsAdapter;

    public ClusteredSuggestionsView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void setSuggestionsAdapter(SuggestionsAdapter<ExpandableListAdapter> adapter) {
        mSuggestionsAdapter = adapter;
        super.setAdapter(adapter == null ? null : adapter.getListAdapter());
    }

    public SuggestionsAdapter<ExpandableListAdapter> getSuggestionsAdapter() {
        return mSuggestionsAdapter;
    }

    public void setLimitSuggestionsToViewHeight(boolean limit) {
        // not supported
    }

    @Override
    public void onFinishInflate() {
        super.onFinishInflate();
        setItemsCanFocus(true);
        setOnGroupClickListener(new OnGroupClickListener(){
            public boolean onGroupClick(
                    ExpandableListView parent, View v, int groupPosition, long id) {
                // disable collapsing / expanding
                return true;
            }});
		setOnGroupExpandListener(new OnGroupExpandListener(){
			@Override
			public void onGroupExpand(int groupPosition) {
				for(int i = 0; i < mSuggestionsAdapter.getListAdapter().getGroupCount();i++)
				{
					if(groupPosition != i) 
					{
						collapseGroup(i);
					}
				}
			}
		});

		setOnFocusChangeListener(new ClusteredSuggestionsFocusListener());
    }

    public void expandAll() {
        if (mSuggestionsAdapter != null) {
            ExpandableListAdapter adapter = mSuggestionsAdapter.getListAdapter();
            for (int i = 0; i < adapter.getGroupCount(); ++i) {
                expandGroup(i);
            }
        }
    }

    /**
     * Hides the input method when the suggestions get focus.
     */
    private class ClusteredSuggestionsFocusListener implements OnFocusChangeListener {
        public void onFocusChange(View v, boolean focused) {

			if (focused) {
				ClusteredSuggestionsView view = (ClusteredSuggestionsView)v;
				int position = view.getPositionForView(findFocus());
				view.setSelection(position);
			}
        }
    }	

}
