

/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: GeneratePattern.java
* Theme: SmartLock
* Functions: onCreate(Bundle)
* Global Variables: toolbar, generate, pattern, CUSTOMER_SPECIFIC_ENDPOINT, COGNITO_POOL_ID, MY_REGION,
* mqttManager, clientId, credentialsProvider
*/

package com.example.user.smartlock;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;
import com.amazonaws.regions.Regions;
import java.util.Random;
import java.util.UUID;

/**
 * Created by user on 19/3/18.
 */

public class GeneratePattern  extends AppCompatActivity {


    Toolbar toolbar;
    private Button generate;
    private TextView pattern;

    private static final String CUSTOMER_SPECIFIC_ENDPOINT = "a1kw7adkqhyy7v.iot.us-east-2.amazonaws.com";

    // Cognito pool ID. For this app, pool needs to be unauthenticated pool with
    // AWS IoT permissions.
    private static final String COGNITO_POOL_ID = "us-east-2:cbd064f2-8dfd-4bf6-89a6-341e4d22d699";

    // Region of AWS IoT
    private static final Regions MY_REGION = Regions.US_EAST_2;

    AWSIotMqttManager mqttManager;
    String clientId;

    CognitoCachingCredentialsProvider credentialsProvider;

        /*
* Function Name: onCreate
* Input: savedInstanceState -> Bundle which stores the state of the activity
* Output: None
* Logic: It loads the saved instance of the activity along with XML layout file.
* It sets up a connection with the aws iot and sends the generated pattern to the shadow link of aws iot.
* Example Call: onCreate(Bundle);
*
*/


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_generatepattern);


        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        generate = (Button) findViewById(R.id.generatepattern);
        pattern = (TextView) findViewById(R.id.pattern);



        clientId = UUID.randomUUID().toString();


        // Initialize the AWS Cognito credentials provider
        credentialsProvider = new CognitoCachingCredentialsProvider(
                getApplicationContext(), // context
                COGNITO_POOL_ID, // Identity Pool ID
                MY_REGION // Region
        );

        // MQTT Client
        mqttManager = new AWSIotMqttManager(clientId, CUSTOMER_SPECIFIC_ENDPOINT);

        try {

            Log.d("msg", "Status = ");
            mqttManager.connect(credentialsProvider, new AWSIotMqttClientStatusCallback() {
                @Override
                public void onStatusChanged(final AWSIotMqttClientStatus status,
                                            final Throwable throwable) {
                    Log.d("Status", "Status = " + String.valueOf(status));

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if (status == AWSIotMqttClientStatus.Connecting) {

                                Log.d("Status","Connecting..." );

                            }
                            else if (status == AWSIotMqttClientStatus.Connected) {

                                Log.d("Status","Connected" );

                            }
                            else if (status == AWSIotMqttClientStatus.Reconnecting) {
                                if (throwable != null) {
                                    Log.d("Status", "Connection error.", throwable);
                                }

                            }
                            else if (status == AWSIotMqttClientStatus.ConnectionLost) {
                                if (throwable != null) {
                                    Log.d("Status", "Connection error.", throwable);
                                    throwable.printStackTrace();
                                }

                            }
                        }
                    });
                }
            });
        } catch (final Exception e) {
            Log.e("error", "Connection error." + e.getMessage());

        }


        generate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Random r = new Random();
                int i1 = r.nextInt(5 - 1) + 1;
                int i2 = r.nextInt(5 - 1) + 1;
                int i3 = r.nextInt(5 - 1) + 1;
                int i4 = r.nextInt(5 - 1) + 1;
                int i5 = r.nextInt(5 - 1) + 1;

                String finalpattern = i1 +"," + i2 +"," + i3 + "," + i4 + "," +i5;

                pattern.setText(finalpattern);
                final String topic = "$aws/things/PubSub/shadow/update";

                final String msg=String.format("{\"state\":{\"desired\":{\"Pattern\":\"%s\"}}}", finalpattern);
                try {
                    mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
                } catch (Exception e) {
                    Log.d("Status", "Publish error.", e);
                }

            }
        });
    }
}
