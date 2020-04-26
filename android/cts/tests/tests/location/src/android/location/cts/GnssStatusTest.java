package android.location.cts;

import android.location.GnssStatus;

public class GnssStatusTest extends GnssTestCase  {

    private static final String TAG = "GnssStatusTest";
    private static final int LOCATION_TO_COLLECT_COUNT = 1;
    private static final int STATUS_TO_COLLECT_COUNT = 3;

  @Override
  protected void setUp() throws Exception {
    super.setUp();
    mTestLocationManager = new TestLocationManager(getContext());
  }

  /**
   * Tests that one can listen for {@link GnssStatus}.
   */
  public void testGnssStatusChanges() throws Exception {
    // Checks if GPS hardware feature is present, skips test (pass) if not,
    // and hard asserts that Location/GPS (Provider) is turned on if is Cts Verifier.
    if (!TestMeasurementUtil.canTestRunOnCurrentDevice(mTestLocationManager,
        TAG, MIN_HARDWARE_YEAR_MEASUREMENTS_REQUIRED, isCtsVerifierTest())) {
      return;
    }

    // Register Gps Status Listener.
    TestGnssStatusCallback testGnssStatusCallback =
        new TestGnssStatusCallback(TAG, STATUS_TO_COLLECT_COUNT);
    mTestLocationManager.registerGnssStatusCallback(testGnssStatusCallback);

    TestLocationListener locationListener = new TestLocationListener(LOCATION_TO_COLLECT_COUNT);
    mTestLocationManager.requestLocationUpdates(locationListener);

    boolean success = testGnssStatusCallback.awaitStart();
    success = success ? testGnssStatusCallback.awaitStatus() : false;
    success = success ? testGnssStatusCallback.awaitTtff() : false;
    mTestLocationManager.removeLocationUpdates(locationListener);
    success = success ? testGnssStatusCallback.awaitStop() : false;
    mTestLocationManager.unregisterGnssStatusCallback(testGnssStatusCallback);

    SoftAssert.failOrWarning(isMeasurementTestStrict(),
        "Time elapsed without getting the right status changes."
            + " Possibly, the test has been run deep indoors."
            + " Consider retrying test outdoors.",
        success);
  }
}
