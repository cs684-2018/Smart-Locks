
/*
* Team Id: NIL
* Author List: Pratik Sanjay Wagh
* Filename: LogAdapter.java
* Theme: SmartLock
* Functions: LogAdapter(CheckLog, List<LogsDO>), onCreateViewHolder(ViewGroup, int),
* onBindViewHolder(ViewHolder, int), getItemCount(), ViewHolder(View)
* Global Variables: context, result
*/


package com.example.user.smartlock;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import java.util.ArrayList;
import java.util.List;

public class LogAdapter extends RecyclerView.Adapter<LogAdapter.ViewHolder> {


    private Context context;
    private List<LogsDO> result = new ArrayList<>();




    public LogAdapter(CheckLog context, List<LogsDO> result)
    {
        this.context= (Context) context;
        this.result=result;

    }

    /*
* Function Name: onCreateViewHolder
* Input: parent -> ViewGroup which stores viewgroup, viewType-> int which contains the viewtype
* Output: ViewHolder
* Logic: It inflates the XML layout and calls the ViewHolder() function
* Example Call: onCreateViewHolder(ViewGroup, int);
*
*/
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(context);
        View view=inflater.inflate(R.layout.listalllogs,null);

        ViewHolder holder = new ViewHolder(view);
        return holder;
    }

    /*
* Function Name: onBindViewHolder
* Input: holder -> ViewHolder it contains the holder which single element in the RecyclerView,
* position-> int which contains the position of the element in the RecyclerView
* Output: None
* Logic: It set the holder value to username and timestamp
* Example Call: onBindViewHolder(ViewHolder, int) ;
*
*/

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        final LogsDO  user = result.get(position);
        holder.username.setText(user.getUserId());
        holder.timeofcreation.setText(user.getTimestamp());

      
    }

     /*
* Function Name: getItemCount
* Input: None
* Output: int
* Logic: It returns the number of items present in a RecyclerView
* Example Call: getItemCount() ;
*
*/

    @Override
    public int getItemCount() {
        return result.size();
    }

    class ViewHolder extends RecyclerView.ViewHolder{

        View mView;

        public TextView username, timeofcreation;

        public ViewHolder(View itemView) {
            super(itemView);
            mView =itemView;

            username = (TextView) itemView.findViewById(R.id.username);
            timeofcreation = (TextView) itemView.findViewById(R.id.timeofcreation);
        }
    }
}
