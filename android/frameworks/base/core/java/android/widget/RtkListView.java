package android.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.widget.Button;
import android.view.View;
import android.widget.ListView;
import android.util.Log;

public class RtkListView extends ListView {

	static final String LOG_TAG = "RtkListView";

    public RtkListView(Context context) {
        super(context);
    }

    public RtkListView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public RtkListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
    /**
     * Determine how much we need to scroll in order to get the next selected view
     * visible, with a fading edge showing below as applicable.  The amount is
     * capped at {@link #getMaxScrollAmount()} .
     *
     * @param direction either {@link android.view.View#FOCUS_UP} or
     *        {@link android.view.View#FOCUS_DOWN}.
     * @param nextSelectedPosition The position of the next selection, or
     *        {@link #INVALID_POSITION} if there is no next selectable position
     * @return The amount to scroll. Note: this is always positive!  Direction
     *         needs to be taken into account when actually scrolling.
     */
	@Override
    public int amountToScroll(int direction, int nextSelectedPosition) {
        final int listBottom = getHeight() - mListPadding.bottom;
        final int listTop = mListPadding.top;

        int numChildren = getChildCount();

        if (direction == View.FOCUS_DOWN) {
            int indexToMakeVisible = numChildren - 1;
            if (nextSelectedPosition != INVALID_POSITION) {
                indexToMakeVisible = nextSelectedPosition - mFirstPosition;
            }
            while (numChildren <= indexToMakeVisible) {
                // Child to view is not attached yet.
                addViewBelow(getChildAt(numChildren - 1), mFirstPosition + numChildren - 1);
                numChildren++;
            }
            final int positionToMakeVisible = mFirstPosition + indexToMakeVisible;
            final View viewToMakeVisible = getChildAt(indexToMakeVisible);

            int goalBottom = listBottom;
            if (positionToMakeVisible < mItemCount - 1) {
                goalBottom -= getArrowScrollPreviewLength();
            }

            if (viewToMakeVisible.getBottom() <= goalBottom) {
                // item is fully visible.
                return 0;
            }

            if (nextSelectedPosition != INVALID_POSITION
                    && (goalBottom - viewToMakeVisible.getTop()) >= getMaxScrollAmount()) {
                // item already has enough of it visible, changing selection is good enough
                return 0;
            }

            //int amountToScroll = (viewToMakeVisible.getBottom() - goalBottom);
			int ItemHeight = viewToMakeVisible.getHeight();
			Log.i(LOG_TAG,"listTop="+listTop+"ItemHeight="+ItemHeight);
            int amountToScroll = ItemHeight - listTop;

            if ((mFirstPosition + numChildren) == mItemCount) {
                // last is last in list -> make sure we don't scroll past it
                final int max = getChildAt(numChildren - 1).getBottom() - listBottom;
                amountToScroll = Math.min(amountToScroll, max);
            }

            return Math.min(amountToScroll, getMaxScrollAmount());
        } else {
            int indexToMakeVisible = 0;
            if (nextSelectedPosition != INVALID_POSITION) {
                indexToMakeVisible = nextSelectedPosition - mFirstPosition;
            }
            while (indexToMakeVisible < 0) {
                // Child to view is not attached yet.
                addViewAbove(getChildAt(0), mFirstPosition);
                mFirstPosition--;
                indexToMakeVisible = nextSelectedPosition - mFirstPosition;
            }
            final int positionToMakeVisible = mFirstPosition + indexToMakeVisible;
            final View viewToMakeVisible = getChildAt(indexToMakeVisible);
            int goalTop = listTop;
            if (positionToMakeVisible > 0) {
                goalTop += getArrowScrollPreviewLength();
            }
            if (viewToMakeVisible.getTop() >= goalTop) {
                // item is fully visible.
                return 0;
            }

            if (nextSelectedPosition != INVALID_POSITION &&
                    (viewToMakeVisible.getBottom() - goalTop) >= getMaxScrollAmount()) {
                // item already has enough of it visible, changing selection is good enough
                return 0;
            }

            int amountToScroll = (goalTop - viewToMakeVisible.getTop());
            if (mFirstPosition == 0) {
                // first is first in list -> make sure we don't scroll past it
                final int max = listTop - getChildAt(0).getTop();
                amountToScroll = Math.min(amountToScroll,  max);
            }
            return Math.min(amountToScroll, getMaxScrollAmount());
        }
    }
}
