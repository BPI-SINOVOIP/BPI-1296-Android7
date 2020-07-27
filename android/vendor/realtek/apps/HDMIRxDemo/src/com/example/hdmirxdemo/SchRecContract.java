package com.example.hdmirxdemo;

import android.provider.BaseColumns;

public final class SchRecContract {

    private SchRecContract() {}

    public static class SchRecEntry implements BaseColumns {

        public static final String TABLE_NAME="SchRecTable";

        public static String _ID="id";

        public static final String COLUMN_NAME_START_YEAR="syear";
        public static final String COLUMN_NAME_START_MONTH="smonth";
        public static final String COLUMN_NAME_START_DAY="sday";

        public static final String COLUMN_NAME_START_HOUR="sh";
        public static final String COLUMN_NAME_START_MIN="sm";

        public static final String COLUMN_NAME_END_YEAR="eyear";
        public static final String COLUMN_NAME_END_MONTH="emonth";
        public static final String COLUMN_NAME_END_DAY="eday";

        public static final String COLUMN_NAME_END_HOUR="eh";
        public static final String COLUMN_NAME_END_MIN="em";

        public static final String COLUMN_NAME_WIDTH="width";
        public static final String COLUMN_NAME_HEIGHT="height";

        public static final String COLUMN_NAME_FORMAT="format";
        public static final String COLUMN_NAME_PATH="path";

        public static final String COLUMN_NAME_COMPARE_TIME="comparetime";

        // more like resolution, bitrate info.

    }

}
