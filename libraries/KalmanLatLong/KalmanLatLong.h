class KalmanLatLong {
        public:
        KalmanLatLong(double Q_metres_per_second_) { 
            MinAccuracy = 0.0001;
            Q_metres_per_second = Q_metres_per_second_; 
            variance = -1; }

        long get_TimeStamp() { return TimeStamp_milliseconds; }
        double get_lat() { return lat; }
        double get_lng() { return lng; }
    //  double get_accuracy() { return (double)sqrt(variance); }

        void SetState(double lat_, double lng_, double accuracy_, long TimeStamp_milliseconds_) {
            lat=lat_; 
            lng=lng_; 
            variance = accuracy_ * accuracy_; 
            TimeStamp_milliseconds=TimeStamp_milliseconds_;
        }

    /// <summary>
    /// Kalman filter processing for lattitude and longitude
    /// </summary>
    /// <param name="lat_measurement_degrees">new measurement of lattidude</param>
    /// <param name="lng_measurement">new measurement of longitude</param>
    /// <param name="accuracy">measurement of 1 standard deviation error in metres</param>
    /// <param name="TimeStamp_milliseconds">time of measurement</param>
    /// <returns>new state</returns>
        void Process(double lat_measurement, double lng_measurement, double accuracy, long TimeStamp_milliseconds_) {
            if (accuracy < MinAccuracy) accuracy = MinAccuracy;
            if (variance < 0) {
            // if variance < 0, object is unitialised, so initialise with current values
                TimeStamp_milliseconds = TimeStamp_milliseconds_;
                lat=lat_measurement; lng = lng_measurement; variance = accuracy*accuracy; 
            } else {
            // else apply Kalman filter methodology

                long TimeInc_milliseconds = TimeStamp_milliseconds_ - TimeStamp_milliseconds;
                if (TimeInc_milliseconds > 0) {
                // time has moved on, so the uncertainty in the current position increases
                    variance += TimeInc_milliseconds * Q_metres_per_second * Q_metres_per_second / 1000;
                    TimeStamp_milliseconds = TimeStamp_milliseconds_;
                // TO DO: USE VELOCITY INFORMATION HERE TO GET A BETTER ESTIMATE OF CURRENT POSITION
            }

            // Kalman gain matrix K = Covarariance * Inverse(Covariance + MeasurementVariance)
            // NB: because K is dimensionless, it doesn't matter that variance has different units to lat and lng
            double K = variance / (variance + accuracy * accuracy);
            // apply K
            lat += K * (lat_measurement - lat);
            lng += K * (lng_measurement - lng);
            // new Covarariance  matrix is (IdentityMatrix - K) * Covarariance 
            variance = (1 - K) * variance;
        }
    }

    private:
        double MinAccuracy;

        double Q_metres_per_second;    
        long TimeStamp_milliseconds;
        double lat;
        double lng;
        double variance; // P matrix.  Negative means object uninitialised.  NB: units irrelevant, as long as same units used throughout
};