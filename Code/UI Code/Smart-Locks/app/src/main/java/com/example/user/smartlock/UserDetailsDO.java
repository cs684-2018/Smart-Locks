
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: UserDetailsDO.java
* Theme: SmartLock
* Functions: UserDetailsDO(), UserDetailsDO(String, String), getUsername(),setUsername(final String),
* getEmailid(), setEmailid(final String), getTimeofcreation(), setTimeofcreation(final String)
* Global Variables: None
*/

package com.example.user.smartlock;

import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBAttribute;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBHashKey;
import com.amazonaws.mobileconnectors.dynamodbv2.dynamodbmapper.DynamoDBTable;


@DynamoDBTable(tableName = "smartlock-mobilehub-1560124461-user_details")

public class UserDetailsDO {
    private String _username;
    private String _emailid;
    private String _timeofcreation;

        /*
* Function Name: UserDetailsDO
* Input: None
* Output: None
* Logic: None
* Example Call: UserDetailsDO();
*
*/

    public UserDetailsDO() {
    }


    /*
* Function Name: UserDetailsDO
* Input: name -> String, timeofcreation -> String
* Output: None
* Logic: It sets the value of the variable
* Example Call: UserDetailsDO(String, String);
*
*/

    public UserDetailsDO(String name, String timeofcreation) {
        this._username = name;
        this._timeofcreation = timeofcreation;

    }

    @DynamoDBHashKey(attributeName = "username")
    @DynamoDBAttribute(attributeName = "username")

       /*
* Function Name: getUsername
* Input: None
* Output: String
* Logic: It returns value of the variable
* Example Call: getUsername();
*
*/

    public String getUsername() {
        return _username;
    }

          /*
* Function Name: setUsername
* Input: _username -> String
* Output: None
* Logic: It sets the value of the variable
* Example Call: setUsername(final String);
*
*/

    public void setUsername(final String _username) {
        this._username = _username;
    }
    @DynamoDBAttribute(attributeName = "emailid")

         /*
* Function Name: getEmailid
* Input: None
* Output: String
* Logic: It returns value of the variable
* Example Call: getEmailid();
*
*/
    public String getEmailid() {
        return _emailid;
    }

             /*
* Function Name: setEmailid
* Input: _username -> String
* Output: None
* Logic: It sets the value of the variable
* Example Call: setEmailid(final String);
*
*/

    public void setEmailid(final String _emailid) {
        this._emailid = _emailid;
    }
    @DynamoDBAttribute(attributeName = "timeofcreation")

          /*
* Function Name: getTimeofcreation
* Input: None
* Output: String
* Logic: It returns value of the variable
* Example Call: getTimeofcreation();
*
*/
    public String getTimeofcreation() {
        return _timeofcreation;
    }

               /*
* Function Name: setTimeofcreation
* Input: _username -> String
* Output: None
* Logic: It sets the value of the variable
* Example Call: setTimeofcreation(final String);
*
*/


    public void setTimeofcreation(final String _timeofcreation) {
        this._timeofcreation = _timeofcreation;
    }

}
