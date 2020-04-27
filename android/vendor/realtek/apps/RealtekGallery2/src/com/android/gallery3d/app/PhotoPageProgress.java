/*
 * Copyright (C) 2012 The Android Open Source Project
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

import android.app.Activity;
import android.content.Context;
import android.os.Handler;  
import android.os.Message;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.android.gallery3d.R;
import com.android.gallery3d.data.MediaItem;
import com.realtek.bitmapfun.util.ImageCache;
import com.realtek.bitmapfun.util.ImageFetcher;
import com.realtek.bitmapfun.util.ImageWorker;
import com.realtek.bitmapfun.util.ReturnSizes;

public class PhotoPageProgress 
{
    private PhotoPage.Model mModel;

    private ViewGroup mParentLayout;
    private ViewGroup mSlideshowContainer;
    private ViewGroup mPhotoInfoContainer;
    private ViewGroup mControlContainer;
    private ViewGroup mPhotoListContainer;
    private ImageView prevPhotoImageView;
    private boolean mVisibility = false;
    private boolean mSlideshowShown = false;
    private boolean mPhotoInfoShown = false;
    private boolean mControlShown = false;
    private boolean mPhotoListShown = false;
    private boolean mSlideshowEnable = false;

    private Animation mContainerAnimIn = new AlphaAnimation(0f, 1f);
    private Animation mContainerAnimOut = new AlphaAnimation(1f, 0f);
    private static final int CONTAINER_ANIM_DURATION_MS = 400;

    private static final int HIDE_GUI_SLIDESHOW = 0x1;
    private static final int HIDE_GUI_PHOTOINFO = 0x2;
    private static final int HIDE_GUI_CONTROL = 0x3;
    private static final int HIDE_GUI_PHOTOLIST = 0x4;
    private static final int CHANGE_PHOTO_VIEW = 0x10;
    private static final int CHANGE_PHOTO_VIEW_TO_NEXT = 0x11;

    private static final int GUI_DELAY_DURATION_MS = 3000;

    private static int m_progress_center_width = 156;
    private static int m_progress_center_height = 117;
    private static int m_progress_center_padding = 42;
    private static int m_progress_other_width = 96;
    private static int m_progress_other_height = 72;

    private FloatPhotoListView floatPhotoListView;
    private FloatPhotoAdapter floatPhotoAdapter;
    private ImageWorker mImageWorker;
    private int mCurrentIndex = 0;
    private int mNextIndex = 0;
    public static final String IMAGE_CACHE_DIR = "images";

    ImageView floatHoverArea;
    ImageView photo_progessbar_fr;
    ImageView photo_progessbar_slideshow_play;
    ImageView photo_progessbar_ff;
    ImageView photo_progessbar_rotation_left;
    ImageView photo_progessbar_rotation_right;
    public GifView gifView;

    Handler mHandler = new Handler()
    {
        public void handleMessage(Message msg)
        {
            switch (msg.what)
            {
                case HIDE_GUI_SLIDESHOW:
                    showSlideshow(false);
                    break;
                case HIDE_GUI_PHOTOINFO:
                    showPhotoInfo(false);
                    break;
                case HIDE_GUI_CONTROL:
                    showControl(false);
                    break;
                case HIDE_GUI_PHOTOLIST:
                    showPhotoList(false);
                    showPhotoInfo(true);
                    break;
                case CHANGE_PHOTO_VIEW:
                    if( mModel instanceof PhotoDataAdapter )
                    {
                        mModel.moveTo(mCurrentIndex);
                    }
                    break;
                case CHANGE_PHOTO_VIEW_TO_NEXT:
                    if( mModel instanceof PhotoDataAdapter )
                    {
                        mModel.moveTo(mNextIndex);
                    }
                default:
                    break;
            }
        }
    };

    public PhotoPageProgress(AbstractGalleryActivity activity, RelativeLayout layout, PhotoPage.Model mModel)
    {
        ViewGroup mContainer;

        this.mModel = mModel;

        m_progress_center_width = (int) activity.getResources().getDimension(R.dimen.photo_progress_center_width);
        m_progress_center_height = (int) activity.getResources().getDimension(R.dimen.photo_progress_center_height);
        m_progress_center_padding = (int) activity.getResources().getDimension(R.dimen.photo_progress_center_padding);
        m_progress_other_width = (int) activity.getResources().getDimension(R.dimen.photo_progress_other_width);
        m_progress_other_height = (int) activity.getResources().getDimension(R.dimen.photo_progress_other_height);
        mParentLayout = layout;
        LayoutInflater inflater = (LayoutInflater) activity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        initImageWorker(activity);
        gifView = new GifView(activity.getAndroidContext());
        mParentLayout.addView(gifView);

        mContainer = (ViewGroup) mParentLayout.findViewById(R.id.photopage_info_display);
        if( mContainer == null )
        {
            mContainer = (ViewGroup)inflater.inflate(R.layout.photopage_info_display, mParentLayout, false);
            mParentLayout.addView(mContainer);
        }

        mSlideshowContainer = (ViewGroup) mContainer.findViewById(R.id.slideshow);
        mPhotoInfoContainer = (ViewGroup) mContainer.findViewById(R.id.photopage_info);

        mPhotoListContainer = (ViewGroup) mParentLayout.findViewById(R.id.floatphoto_layout);
        if( mPhotoListContainer == null )
        {
            mPhotoListContainer = (ViewGroup)inflater.inflate(R.layout.floatphoto_listview, mParentLayout, false);
            mParentLayout.addView(mPhotoListContainer);
        }

        mContainer = (ViewGroup) mParentLayout.findViewById(R.id.photopage_progress);
        if( mContainer == null )
        {
            mContainer = (ViewGroup)inflater.inflate(R.layout.photopage_progress, mParentLayout, false);
            mParentLayout.addView(mContainer);
        }

        mControlContainer = (ViewGroup) mContainer.findViewById(R.id.cursor_progress);
        floatHoverArea = (ImageView)mContainer.findViewById(R.id.floatHoverArea);
        floatHoverArea.setOnHoverListener(new FloatAreaHoverListener());

        mContainerAnimIn.setDuration(CONTAINER_ANIM_DURATION_MS);
        mContainerAnimOut.setDuration(CONTAINER_ANIM_DURATION_MS);

        floatPhotoListView = (FloatPhotoListView) mPhotoListContainer.findViewById(R.id.floatPhotoListView);
        floatPhotoListView.setOnItemSelectedListener(new PhotoItemSelectedListener());
        floatPhotoListView.setOnItemClickListener(new PhotoItemClickListener());
        floatPhotoListView.setSpacing((int)activity.getResources().getDimension(R.dimen.photo_progress_other_spacing));

        photo_progessbar_fr = (ImageView)mControlContainer.findViewById(R.id.photo_progessbar_fr);
        photo_progessbar_slideshow_play = (ImageView)mControlContainer.findViewById(R.id.photo_progessbar_slideshow_play);
        photo_progessbar_ff = (ImageView)mControlContainer.findViewById(R.id.photo_progessbar_ff);
        photo_progessbar_rotation_left = (ImageView)mControlContainer.findViewById(R.id.photo_progessbar_rotation_left);
        photo_progessbar_rotation_right = (ImageView)mControlContainer.findViewById(R.id.photo_progessbar_rotation_right);

        photo_progessbar_fr.setOnHoverListener(new PhotoPageControlHoverListener());
        photo_progessbar_slideshow_play.setOnHoverListener(new PhotoPageControlHoverListener());
        photo_progessbar_ff.setOnHoverListener(new PhotoPageControlHoverListener());
        photo_progessbar_rotation_left.setOnHoverListener(new PhotoPageControlHoverListener());
        photo_progessbar_rotation_right.setOnHoverListener(new PhotoPageControlHoverListener());

    }

    public void show(Boolean enable)
    {
        if(enable==true)
        {
            if(mVisibility==false)
            {
                mVisibility=true;
            }
            mSlideshowEnable = false;
            setSlideshowInfo(mSlideshowEnable);
            showSlideshow(true);
            floatHoverArea.setFocusable(true);
            floatHoverArea.setFocusableInTouchMode(true);
            floatHoverArea.requestFocus();
            if(gifView!=null) {
                String photoList[] = getPhotoList();
                if(photoList!=null) {
                    gifView.setFilePath(photoList[mCurrentIndex]);
                }
            }
        }
        else
        {
            if(mVisibility==true)
            {
                mVisibility=false;
            }
            showPhotoInfo(false);
            showControl(false);
            showPhotoList(false);
            floatHoverArea.clearFocus();
            if(gifView!=null) {
                gifView.hide();
            }
        }
    }

    public void setClickListener(View.OnClickListener listener)
    {
        if( photo_progessbar_slideshow_play!=null )
        {
            photo_progessbar_slideshow_play.setOnClickListener(listener);
        }
        if( photo_progessbar_fr!=null )
        {
            photo_progessbar_fr.setOnClickListener(listener);
        }
        if( photo_progessbar_ff!=null )
        {
            photo_progessbar_ff.setOnClickListener(listener);
        }
        if( photo_progessbar_rotation_left!=null )
        {
            photo_progessbar_rotation_left.setOnClickListener(listener);
        }
        if( photo_progessbar_rotation_right!=null )
        {
            photo_progessbar_rotation_right.setOnClickListener(listener);
        }
    }

    public void setKeyListener(View.OnKeyListener listener)
    {
        if( floatHoverArea!=null )
        {
            floatHoverArea.setOnKeyListener(listener);
        }
    }

    public void showSlideshow(Boolean enable)
    {
        if(enable==true)
        {
            setSlideshowInfo(mSlideshowEnable);
            if(mSlideshowShown==false)
            {
                mSlideshowContainer.clearAnimation();
                mContainerAnimIn.reset();
                mSlideshowContainer.startAnimation(mContainerAnimIn);
                mSlideshowContainer.setVisibility(View.VISIBLE);
                mSlideshowShown=true;
            }
            mHandler.removeMessages(HIDE_GUI_SLIDESHOW);
            mHandler.sendEmptyMessageDelayed(HIDE_GUI_SLIDESHOW, GUI_DELAY_DURATION_MS);
        }
        else
        {
            if(mSlideshowShown==true)
            {
                mSlideshowContainer.clearAnimation();
                mContainerAnimOut.reset();
                mSlideshowContainer.startAnimation(mContainerAnimOut);
                mSlideshowContainer.setVisibility(View.INVISIBLE);
                mSlideshowShown=false;
            }
        }
    }

    public void showPhotoInfo(Boolean enable)
    {
        if(enable==true)
        {
            if(mPhotoListShown==true || mControlShown==true || mSlideshowEnable==true)
            {
                return;
            }
            if(mVisibility==true && mPhotoInfoShown==false)
            {
                mPhotoInfoContainer.clearAnimation();
                mContainerAnimIn.reset();
                mPhotoInfoContainer.startAnimation(mContainerAnimIn);
                mPhotoInfoContainer.setVisibility(View.VISIBLE);
                mPhotoInfoShown=true;
            }
            mHandler.removeMessages(HIDE_GUI_PHOTOINFO);
            mHandler.sendEmptyMessageDelayed(HIDE_GUI_PHOTOINFO, GUI_DELAY_DURATION_MS);
        }
        else
        {
            if(mPhotoInfoShown==true)
            {
                mPhotoInfoContainer.clearAnimation();
                mContainerAnimOut.reset();
                mPhotoInfoContainer.startAnimation(mContainerAnimOut);
                mPhotoInfoContainer.setVisibility(View.INVISIBLE);
                mPhotoInfoShown=false;
            }
        }
    }

    public void showControl(Boolean enable)
    {
        if(enable==true)
        {
            if(mVisibility==true && mControlShown==false)
            {
                mControlContainer.clearAnimation();
                mContainerAnimIn.reset();
                mControlContainer.startAnimation(mContainerAnimIn);
                mControlContainer.setVisibility(View.VISIBLE);
                mControlShown=true;
            }
            mHandler.removeMessages(HIDE_GUI_CONTROL);
            mHandler.sendEmptyMessageDelayed(HIDE_GUI_CONTROL, GUI_DELAY_DURATION_MS);
        }
        else
        {
            if(mControlShown==true)
            {
                mControlContainer.clearAnimation();
                mContainerAnimOut.reset();
                mControlContainer.startAnimation(mContainerAnimOut);
                mControlContainer.setVisibility(View.INVISIBLE);
                mControlShown=false;
            }
        }
    }

    public void showPhotoList(Boolean enable)
    {
        if(enable==true)
        {
            if(mSlideshowEnable==true)
            {
                return;
            }
            if(mVisibility==true && mPhotoListShown==false)
            {
                showPhotoInfo(false);
                if( floatPhotoAdapter!=null && floatPhotoAdapter.getCount()>1 )
                {
                    mPhotoListContainer.clearAnimation();
                    mContainerAnimIn.reset();
                    mPhotoListContainer.startAnimation(mContainerAnimIn);
                    mPhotoListContainer.setVisibility(View.VISIBLE);
                    mPhotoListShown=true;
                }
                else
                {
                    mPhotoListContainer.setVisibility(View.INVISIBLE);
                }
            }
            mHandler.removeMessages(HIDE_GUI_PHOTOLIST);
            if(mControlShown==false) {
                mHandler.sendEmptyMessageDelayed(HIDE_GUI_PHOTOLIST, GUI_DELAY_DURATION_MS);
            }
        }
        else
        {
            if(mPhotoListShown==true)
            {
                mPhotoListContainer.clearAnimation();
                mContainerAnimOut.reset();
                mPhotoListContainer.startAnimation(mContainerAnimOut);
                mPhotoListContainer.setVisibility(View.INVISIBLE);
                mPhotoListShown=false;
            }
        }
    }

    public void onPrevious()
    {
        Log.d("PhotoPageProgress", " onPrevious()");
        if( mCurrentIndex>0 ) {
            setPhotoListSelection(mCurrentIndex-1);
            showPhotoList(true);
        }
    }

    public void onNext()
    {
        Log.d("PhotoPageProgress", " onNext()");
        if(mCurrentIndex<getPhotoList().length-1) {
            setPhotoListSelection(mCurrentIndex+1);
            showPhotoList(true);
        }
    }

    public void setPhotoListSelection(int currentIndex)
    {
        if (mCurrentIndex == currentIndex) return;
        floatPhotoListView.setSelection(currentIndex);
        mCurrentIndex = currentIndex;
        if(gifView!=null) {
            String photoList[] = getPhotoList();
            if(photoList!=null) {
                gifView.setFilePath(photoList[mCurrentIndex]);
            }
        }
    }

    public void setPhotoList(String[] photoList)
    {
        floatPhotoAdapter = new FloatPhotoAdapter(mImageWorker, photoList, m_progress_other_width, m_progress_other_height);
        floatPhotoListView.setAdapter( floatPhotoAdapter );
        if(gifView!=null) {
            if(photoList!=null) {
                gifView.setFilePath(photoList[mCurrentIndex]);
            }
        }
    }

    public String[] getPhotoList()
    {
        if(floatPhotoAdapter!=null)
        {
            return floatPhotoAdapter.getTotalItem();
        }
        return null;
    }

    public void setPhotoInfo(String name, String Dimension, int index)
    {
        TextView photopage_info_name = (TextView) mPhotoInfoContainer.findViewById(R.id.photopage_info_name);
        TextView photopage_info_dimension = (TextView) mPhotoInfoContainer.findViewById(R.id.photopage_info_dimension);
        TextView photopage_info_count = (TextView) mPhotoInfoContainer.findViewById(R.id.photopage_info_count);

        if(name!=null)
        {
            photopage_info_name.setText(name);
        }
        else
        {
            photopage_info_name.setText("");
        }
        if(Dimension!=null)
        {
            photopage_info_dimension.setText(Dimension);
        }
        else
        {
            photopage_info_dimension.setText("");
        }
        if(floatPhotoAdapter!=null && floatPhotoAdapter.getCount()>1)
        {
            photopage_info_count.setText(index+"/"+floatPhotoAdapter.getCount());
        }
        else
        {
            photopage_info_count.setText("");
        }
        setSlideshowInfo(mSlideshowEnable);
    }

    public void setSlideshowInfo(Boolean isSlideshow)
    {
        TextView slideshow_info = (TextView) mSlideshowContainer.findViewById(R.id.slideshow_info);
        if(isSlideshow==true)
            slideshow_info.setText("on");
        else
            slideshow_info.setText("off");
    }

    protected void initImageWorker(Activity activity)
    {
        ReturnSizes mReturnSizes =  new ReturnSizes(m_progress_center_width, m_progress_center_height);
        mImageWorker = new ImageFetcher(activity, null, mReturnSizes.getWidth(), mReturnSizes.getHeight());
        mImageWorker.setImageCache(ImageCache.findOrCreateCache(activity, IMAGE_CACHE_DIR));
        mImageWorker.setImageFadeIn(false);
//        mImageWorker.setLoadingImage(R.drawable.spinner_76_inner_holo);
    }

    public Boolean slideshowEnable(Boolean enable)
    {
        if(floatPhotoAdapter!=null && floatPhotoAdapter.getCount()>1)
        {
            if( mSlideshowEnable==false && enable==true)
            {
                mSlideshowEnable=true;
                showPhotoInfo(false);
                showPhotoList(false);
                showControl(false);
                showSlideshow(true);
                return true;
            }
            else if( mSlideshowEnable==true && enable==false)
            {
                mSlideshowEnable=false;
                return true;
            }
        }
        return false;
    }

    public Boolean getSlideshowEnable()
    {
        return mSlideshowEnable;
    }

    public void cancelWork()
    {
        int childCount = floatPhotoListView.getChildCount();
        for(int i = 0; i < childCount; i++) {
            ImageView imageView = (ImageView)(floatPhotoListView.getChildAt(i));
            if (mImageWorker!=null) {
                mImageWorker.cancelWork(imageView);
            }
        }
    }

    private class PhotoItemSelectedListener implements AdapterView.OnItemSelectedListener
    {
        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
        {
            if(view==null)
            {
                return;
            }
            ImageView imageView = (ImageView)view;;
            imageView.setPaddingRelative(0,0,0,0);
            if(prevPhotoImageView != null && imageView != null && imageView.hashCode() != prevPhotoImageView.hashCode())
            {
                //reduce center item
                prevPhotoImageView.setLayoutParams(new FloatPhotoListView.LayoutParams(m_progress_other_width, m_progress_other_height));
                prevPhotoImageView.setPaddingRelative(0,0,0,0);
            }
            prevPhotoImageView = imageView ;
            //enlarge center item
            imageView.setLayoutParams(new FloatPhotoListView.LayoutParams(m_progress_center_width+m_progress_center_padding*2, m_progress_center_height));
            prevPhotoImageView.setPaddingRelative(m_progress_center_padding,0,m_progress_center_padding,0);

            //Update photo view
            mCurrentIndex = position;
            mHandler.removeMessages(CHANGE_PHOTO_VIEW);
            mHandler.sendEmptyMessageDelayed(CHANGE_PHOTO_VIEW, 0);
        }
        @Override
        public void onNothingSelected(AdapterView<?> arg0)
        {
            // TODO Auto-generated method stub
        }
    }

    private class PhotoItemClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id)
        {
            mNextIndex = position;
            mHandler.removeMessages(CHANGE_PHOTO_VIEW_TO_NEXT);
            mHandler.sendEmptyMessageDelayed(CHANGE_PHOTO_VIEW_TO_NEXT, 500);
        }
    }

    private class PhotoPageControlHoverListener implements View.OnHoverListener
    {
        @Override
        public boolean onHover(View view, MotionEvent event)
        {
            boolean hasFocus=false;
            if(event.getAction() == MotionEvent.ACTION_HOVER_ENTER)
            {
                hasFocus=true;
            }
            else if(event.getAction() == MotionEvent.ACTION_HOVER_EXIT)
            {
                hasFocus=false;
            }
            else
            {
                return false;
            }

            switch( view.getId() )
            {
                case R.id.photo_progessbar_fr:
                    if(floatPhotoAdapter!=null)
                    {
                        if( mCurrentIndex>0 && hasFocus )
                            photo_progessbar_fr.setImageResource(R.drawable.photo_progessbar_fr_focus);
                        else
                            photo_progessbar_fr.setImageResource(R.drawable.photo_progessbar_fr);
                    }
                    break;
                case R.id.photo_progessbar_slideshow_play:
                    if(floatPhotoAdapter!=null)
                    {
                        if( floatPhotoAdapter.getCount()>1 && hasFocus )
                            photo_progessbar_slideshow_play.setImageResource(R.drawable.photo_progessbar_slideshow_play_focus);
                        else
                            photo_progessbar_slideshow_play.setImageResource(R.drawable.photo_progessbar_slideshow_play);
                    }
                    break;
                case R.id.photo_progessbar_ff:
                    if(floatPhotoAdapter!=null)
                    {
                        if( mCurrentIndex<floatPhotoAdapter.getCount()-1 && hasFocus )
                            photo_progessbar_ff.setImageResource(R.drawable.photo_progessbar_ff_focus);
                        else
                            photo_progessbar_ff.setImageResource(R.drawable.photo_progessbar_ff);
                    }
                    break;
                case R.id.photo_progessbar_rotation_left:
                    if( mModel!=null && mModel.getMediaItem(0)!=null && mModel.getLoadingState(0)==PhotoPage.Model.LOADING_COMPLETE
                        && (mModel.getMediaItem(0).getSupportedOperations()&MediaItem.SUPPORT_ROTATE)!=0 && hasFocus )
                        photo_progessbar_rotation_left.setImageResource(R.drawable.photo_progessbar_rotation_left_f);
                    else
                        photo_progessbar_rotation_left.setImageResource(R.drawable.photo_progessbar_rotation_left);
                    break;
                case R.id.photo_progessbar_rotation_right:
                    if( mModel!=null && mModel.getMediaItem(0)!=null && mModel.getLoadingState(0)==PhotoPage.Model.LOADING_COMPLETE
                        && (mModel.getMediaItem(0).getSupportedOperations()&MediaItem.SUPPORT_ROTATE)!=0 && hasFocus )
                        photo_progessbar_rotation_right.setImageResource(R.drawable.photo_progessbar_rotation_right_f);
                    else
                        photo_progessbar_rotation_right.setImageResource(R.drawable.photo_progessbar_rotation_right);
                    break;
                default:
                    break;
            }
            return true;
        }
    }

    private class FloatAreaHoverListener implements View.OnHoverListener
    {
        @Override
        public boolean onHover(View view, MotionEvent event)
        {
            mHandler.removeMessages(HIDE_GUI_CONTROL);
            mHandler.removeMessages(HIDE_GUI_PHOTOLIST);
            if(event.getAction() == MotionEvent.ACTION_HOVER_ENTER && mVisibility==true)
            {
                showControl(true);
                showPhotoList(true);
            }
            else if(event.getAction() == MotionEvent.ACTION_HOVER_EXIT)
            {
                mHandler.sendEmptyMessageDelayed(HIDE_GUI_CONTROL, GUI_DELAY_DURATION_MS);
                mHandler.sendEmptyMessageDelayed(HIDE_GUI_PHOTOLIST, GUI_DELAY_DURATION_MS);
            }
            else
            {
                showControl(true);
                showPhotoList(true);
                return false;
            }
            return true;
        }
    }
}
