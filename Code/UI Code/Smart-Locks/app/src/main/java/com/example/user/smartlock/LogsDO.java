
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: LogsDO.java
* Theme: SmartLock
* Functions: getTimestamp(), setTimestamp(final String), getUserId(), setUserId(final String)
* Global Variables: _timestamp, _userId
*/

package com.example.user.smartlock;

import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBAttribute;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBHashKey;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBTable;


@DynamoDBTable(tableName = "smartlock-mobilehub-1560124461-logs")

public class LogsDO {
    private String _timestamp;
    private String _userId;

    @DynamoDBHashKey(attributeName = "timestamp")
    @DynamoDBAttribute(attributeName = "timestamp")

       /*
* Function Name: getTimestamp
* Input: None
* Output: String
* Logic: It returns the _timestamp
* Example Call: getTimestamp();
*
*/
    public String getTimestamp() {
        return _timestamp;
    }

        /*
* Function Name: setTimestamp
* Input: _timestamp -> String
* Output: None
* Logic: Setting the value of the _timestamp to _timestamp parameter value
* Example Call: setTimestamp(final String);
*
*/

    public void setTimestamp(final String _timestamp) {
        this._timestamp = _timestamp;
    }
    @DynamoDBAttribute(attributeName = "userId")

         /*
* Function Name: getUserId
* Input: None
* Output: String
* Logic: It returns the _userId
* Example Call: getUserId();
*
*/
    public String getUserId() {
        return _userId;
    }

        /*
* Function Name: setUserId
* Input: _userId -> String
* Output: None
* Logic: Setting the value of the _userId to _userId parameter value
* Example Call: setUserId(final String);
*
*/

    public void setUserId(final String _userId) {
        this._userId = _userId;
    }

}
