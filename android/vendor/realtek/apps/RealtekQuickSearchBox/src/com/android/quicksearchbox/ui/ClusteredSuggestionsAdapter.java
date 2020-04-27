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

import com.android.quicksearchbox.CorpusResult;
import com.android.quicksearchbox.ListSuggestionCursor;
import com.android.quicksearchbox.R;
import com.android.quicksearchbox.Suggestion;
import com.android.quicksearchbox.SuggestionCursor;
import com.android.quicksearchbox.SuggestionPosition;
import com.android.quicksearchbox.SuggestionUtils;
import com.android.quicksearchbox.Suggestions;
import com.android.quicksearchbox.Corpus;


import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnTouchListener;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashSet;
import android.provider.MediaStore;

/**
 * Adapter for suggestions list where suggestions are clustered by corpus.
 */
public class ClusteredSuggestionsAdapter extends SuggestionsAdapterBase<ExpandableListAdapter> {

    private static final String TAG = "QSB.ClusteredSuggestionsAdapter";

    private final static int GROUP_SHIFT = 32;
    private final static long CHILD_MASK = 0xffffffff;

    private final Adapter mAdapter;
    private final Context mContext;
    private final LayoutInflater mInflater;

    public ClusteredSuggestionsAdapter(SuggestionViewFactory viewFactory, Context context) {
        super(viewFactory);
        mAdapter = new Adapter();
        mContext = context;
        mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public boolean isEmpty() {
        return mAdapter.getGroupCount() == 0;
    }

    @Override
    public boolean willPublishNonPromotedSuggestions() {
        return true;
    }

    @Override
    public SuggestionPosition getSuggestion(long suggestionId) {
        return mAdapter.getChildById(suggestionId);
    }

    @Override
    public ExpandableListAdapter getListAdapter() {
        return mAdapter;
    }

    @Override
    protected void notifyDataSetChanged() {
        mAdapter.buildCorpusGroups();
        mAdapter.notifyDataSetChanged();
    }

    @Override
    protected void notifyDataSetInvalidated() {
        mAdapter.buildCorpusGroups();
        mAdapter.notifyDataSetInvalidated();
    }

    private class Adapter extends BaseExpandableListAdapter {

        private ArrayList<SuggestionCursor> mCorpusGroups;

        public void buildCorpusGroups() {
            Suggestions suggestions = getSuggestions();
            SuggestionCursor promoted = getCurrentPromotedSuggestions();
            HashSet<String> promotedSuggestions = new HashSet<String>();
            if (promoted != null && promoted.getCount() > 0) {
                promoted.moveTo(0);
                do {
                    promotedSuggestions.add(SuggestionUtils.getSuggestionKey(promoted));
                } while (promoted.moveToNext());
            }
            if (suggestions == null) {
                mCorpusGroups = null;
            } else {
                if (mCorpusGroups == null) {
                    mCorpusGroups = new ArrayList<SuggestionCursor>();
                } else {
                    mCorpusGroups.clear();
                }

				for(int count=0;count<4;count++)
				{
					mCorpusGroups.add(null);
				}
				
                for (CorpusResult result : suggestions.getCorpusResults()) {
                    ListSuggestionCursor corpusSuggestions = new ListSuggestionCursor(result.getUserQuery());

                    if (result.getCount() > 0)
                    {
                        result.moveTo(0);
                        Corpus corpus = result.getCorpus();
//                        Log.d(TAG, "corpus = " + corpus.getName());
                        boolean bIsGallery = corpus.getName().contains("gallery3d");
                        boolean videoFirst = true;
/*
                        if (bIsGallery)
                        {
                            if (result.getSuggestionIntentDataString().startsWith(MediaStore.Video.Media.EXTERNAL_CONTENT_URI.toString()))
                                videoFirst = true;
                        }
*/
                        for (int i = 0; i < result.getCount(); ++i) {
                            result.moveTo(i);
                            if (!result.isWebSearchSuggestion()) {
                                if (!promotedSuggestions.contains(SuggestionUtils.getSuggestionKey(result)))
                                {
                                    if (bIsGallery)
                                    {
                                        if ((videoFirst == true && 
                                            result.getSuggestionIntentDataString().startsWith(MediaStore.Images.Media.EXTERNAL_CONTENT_URI.toString())) ||
                                            (videoFirst == false && 
                                            result.getSuggestionIntentDataString().startsWith(MediaStore.Video.Media.EXTERNAL_CONTENT_URI.toString()))
                                            )
                                        {
                                            //convert to video form image or convert to image form video, add the new group 
                                            mCorpusGroups.remove(0);
                                            mCorpusGroups.add(0, corpusSuggestions);
                                            corpusSuggestions = new ListSuggestionCursor(result.getUserQuery());
                                            bIsGallery = false;
                                        }
                                    }
                                    corpusSuggestions.add(new SuggestionPosition(result, i));
                                }
                            }
                        }
						if( result.getCorpus().getName().contains("gallery3d") )
						{
                                if ((videoFirst == true &&
                                result.getSuggestionIntentDataString().startsWith(MediaStore.Video.Media.EXTERNAL_CONTENT_URI.toString())) ||
                                (videoFirst == false &&
                                result.getSuggestionIntentDataString().startsWith(MediaStore.Images.Media.EXTERNAL_CONTENT_URI.toString()))
                                )
                                {
                                mCorpusGroups.remove(0);
                                mCorpusGroups.add(0, corpusSuggestions);
                                }
                                else
                                {
                                mCorpusGroups.remove(1);
                                mCorpusGroups.add(1, corpusSuggestions);
                                }
						}
						else if( result.getCorpus().getName().contains("music") )
						{
							mCorpusGroups.remove(2);
							mCorpusGroups.add(2, corpusSuggestions);
						}
						else if( result.getCorpus().getName().contains("apps") )
						{
							mCorpusGroups.remove(3);
							mCorpusGroups.add(3, corpusSuggestions);
						}
						else if(!result.isWebSearchSuggestion())
						{
							Log.d(TAG, "mCorpusGroups.add corpus = " + corpus.getName());
							mCorpusGroups.add(corpusSuggestions);
						}
                    }
                }
            }
        }

        @Override
        public long getCombinedChildId(long groupId, long childId) {
            // add one to the child ID to ensure that the group elements do not have the same ID
            // as the first child within the group.
            return (groupId << GROUP_SHIFT) | ((childId + 1) & CHILD_MASK);
        }

        @Override
        public long getCombinedGroupId(long groupId) {
            return groupId << GROUP_SHIFT;
        }

        public int getChildPosition(long childId) {
            return (int) (childId & CHILD_MASK) - 1;
        }

        public int getGroupPosition(long childId) {
            return (int) ((childId >> GROUP_SHIFT) & CHILD_MASK);
        }

        @Override
        public Suggestion getChild(int groupPosition, int childPosition) {
            SuggestionCursor c = getGroup(groupPosition);
            if (c != null) {
                c.moveTo(childPosition);
                return new SuggestionPosition(c, childPosition);
            }
            return null;
        }

        public SuggestionPosition getChildById(long childId) {
            SuggestionCursor groupCursor = getGroup(getGroupPosition(childId));
            if (groupCursor != null) {
                return new SuggestionPosition(groupCursor, getChildPosition(childId));
            } else {
                Log.w(TAG, "Invalid childId " + Long.toHexString(childId) + " (invalid group)");
                return null;
            }
        }

        @Override
        public long getChildId(int groupPosition, int childPosition) {
            return childPosition;
        }

        @Override
        public View getChildView(int groupPosition, int childPosition, boolean isLastChild,
                View convertView, ViewGroup parent) {
            SuggestionCursor cursor = getGroup(groupPosition);
            if (cursor == null) return null;

			View view=getView(cursor, childPosition, getCombinedChildId(groupPosition, childPosition),
                    convertView, parent);
            return view;
        }

        @Override
        public int getChildrenCount(int groupPosition) {
            SuggestionCursor group = getGroup(groupPosition);
            return group == null ? 0 : group.getCount();
        }

        @Override
        public SuggestionCursor getGroup(int groupPosition) {
            if (groupPosition < promotedGroupCount()) {
                return getCurrentPromotedSuggestions();
            } else {
                int pos = groupPosition - promotedGroupCount();
                if ((pos < 0 ) || (pos >= mCorpusGroups.size())) return null;
                return mCorpusGroups.get(pos);
            }
        }

        private int promotedCount() {
            SuggestionCursor promoted = getCurrentPromotedSuggestions();
            return (promoted == null ? 0 : promoted.getCount());
        }

        private int promotedGroupCount() {
            return (promotedCount() == 0) ? 0 : 1;
        }

        private int corpusGroupCount() {
            return mCorpusGroups == null ? 0 : mCorpusGroups.size();
        }

        @Override
        public int getGroupCount() {
            return promotedGroupCount() + corpusGroupCount();
        }

        @Override
        public long getGroupId(int groupPosition) {
            return groupPosition;
        }

        @Override
        public View getGroupView(
                int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {
            if (convertView == null) {
                convertView = mInflater.inflate(R.layout.suggestion_group, parent, false);
            }
            convertView.getLayoutParams().height = mContext.getResources().
            		getDimensionPixelSize(R.dimen.suggestion_group_spacing);
            // since we've fiddled with the layout params:
            convertView.requestLayout();

			TextView group_text = (TextView) convertView.findViewById(R.id.group_text);
			ImageView group_icon3 = (ImageView) convertView.findViewById(R.id.group_icon3);
			TextView group_count = (TextView) convertView.findViewById(R.id.group_count);

			switch(groupPosition)
			{
				case 0:
					group_text.setText(mContext.getResources().getString(R.string.video));
					group_icon3.setImageResource(R.drawable.us_title_video_none);
					break;
				case 1:
					group_text.setText(mContext.getResources().getString(R.string.photo));
					group_icon3.setImageResource(R.drawable.us_title_photo_none);
					break;
				case 2:
					group_text.setText(mContext.getResources().getString(R.string.music));
					group_icon3.setImageResource(R.drawable.us_title_music_none);
					break;
				default:
					group_text.setText(mContext.getResources().getString(R.string.apps));
					group_icon3.setImageResource(R.drawable.us_title_apps_none);
					break;
			}
			group_count.setText(String.valueOf(getChildrenCount(groupPosition)));

			convertView.setOnClickListener(new GroupClickListener());
			convertView.setOnFocusChangeListener(new GroupFocusListener());
			convertView.setOnTouchListener(new GroupTouchListener());
            return convertView;
        }
        @Override
        public boolean hasStableIds() {
            return false;
        }

        @Override
        public boolean isChildSelectable(int groupPosition, int childPosition) {
            return true;
        }

        @Override
        public int getChildType(int groupPosition, int childPosition) {
            return getSuggestionViewType(getGroup(groupPosition), childPosition);
        }

        @Override
        public int getChildTypeCount() {
            return getSuggestionViewTypeCount();
        }

		public class GroupClickListener implements View.OnClickListener{
			@Override
			public void onClick(View view) {
				ExpandableListView parent = (ExpandableListView)(view.getParent());

				if(parent==null)
					return;

				int position = parent.getPackedPositionGroup(parent.getSelectedPosition());
				if(parent.isGroupExpanded(position))
				{
					parent.collapseGroup(position);
				}
				else
				{
					if(getChildrenCount(position)>0)
						parent.expandGroup(position);
				}
				parent.requestFocus();
			}
		}

		public class GroupTouchListener implements View.OnTouchListener{
			@Override
			public boolean onTouch(View view, MotionEvent motion) {
				if( motion.getAction()== MotionEvent.ACTION_DOWN )
				{
					ExpandableListView parent = (ExpandableListView)(view.getParent());
					if(parent==null)
						return false;

					view.requestFocusFromTouch();

					int position = parent.getPackedPositionGroup(parent.getSelectedPosition());
					if(parent.isGroupExpanded(position))
					{
						parent.collapseGroup(position);
					}
					else
					{
						if(getChildrenCount(position)>0)
							parent.expandGroup(position);
					}
					parent.requestFocus();
					view.requestFocusFromTouch();
				}
				return true;
			}
		}

	    private class GroupFocusListener implements View.OnFocusChangeListener {
	        int groupPosition=0;
	        public void onFocusChange(View v, boolean focused) {

				ClusteredSuggestionsView parent = (ClusteredSuggestionsView)v.getParent();
				ImageView group_icon3 = (ImageView) v.findViewById(R.id.group_icon3);

				if(parent==null)
					return;

				if(focused)
				{
					int itemPosition = parent.getPositionForView(parent.findFocus());
					groupPosition = parent.getPackedPositionGroup(parent.getExpandableListPosition(itemPosition));
					parent.setSelection(itemPosition);
				}
//				Log.d(TAG, "Group focus change, now: " + focused + ", groupPosition: " + groupPosition);
			
				switch(groupPosition)
				{
					case 0:
						if(focused)
						{
							group_icon3.setImageResource(R.drawable.us_title_video_focus);
						}
						else
						{
							group_icon3.setImageResource(R.drawable.us_title_video_none);
						}
						break;
					case 1:
						if(focused)
						{
							group_icon3.setImageResource(R.drawable.us_title_photo_focus);
						}
						else
						{
							group_icon3.setImageResource(R.drawable.us_title_photo_none);
						}
						break;
					case 2:
						if(focused)
						{
							group_icon3.setImageResource(R.drawable.us_title_music_focus);
						}
						else
						{
							group_icon3.setImageResource(R.drawable.us_title_music_none);
						}
						break;
					case 3:
						if(focused)
						{
							group_icon3.setImageResource(R.drawable.us_title_apps_focus);
						}
						else
						{
							group_icon3.setImageResource(R.drawable.us_title_apps_none);
						}
						break;
					default:
						break;
				}
	        }
	    }			
    }
}
