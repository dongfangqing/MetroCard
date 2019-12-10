package com.example.mealcard;


import com.example.socket.MySocket;
import com.example.svner.ScannerActivity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity implements OnClickListener{

    private Button bt_scan;
	private TextView tv_money;
	private Button bt_xj;
	private MySocket ss;
	private Handler handler;
	private String recv_Mag;
    private SharedPreferences sp;
	private Editor editor;
	private String rMB_Value ;
	private String menoy_p = null;
	private boolean flag = true;
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        sp = getSharedPreferences("mysp", MODE_PRIVATE);
        editor = sp.edit();
        
        
     
        ss = new MySocket();             //获得网络对象
        Intent intent = getIntent();        //初始意图
        String rMB_Value = intent.getStringExtra("RMB"); 
//        
//        Toast.makeText(MainActivity.this, rMB_Value  + "222", Toast.LENGTH_SHORT).show();
        
//        rMB_Value  = it.getStringExtra("RMB");
        if(rMB_Value == null || rMB_Value.length() <= 0)
        {
        	
        }else
        {
 //         tv_money.setText(rMB_Value );
   //       Toast.makeText(MainActivity.this, rMB_Value  + "222", Toast.LENGTH_SHORT).show();
        }
  
        //启动获取数据的线程
        new ReadThread().start();  
 

       
        bt_scan = (Button)findViewById(R.id.bt_Scan);         //扫描按键
        bt_xj = (Button)findViewById(R.id.bt_js);           //学校介绍按键
        
        
        tv_money = (TextView)findViewById(R.id.tv_q);       //监听TextView
               

        bt_xj.setOnClickListener(this);

        //进行扫码付款
        bt_scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //跳转到扫码付款
                Intent inten = new Intent(MainActivity.this, ScannerActivity.class);
                startActivity(inten);

            }
        });     
        

        menoy_p  = sp.getString("name", "");
        if(menoy_p == null || menoy_p.length() <= 0)
        {
        	
        }else
        {
          tv_money.setText(menoy_p );
//          Toast.makeText(MainActivity.this, menoy_p  + "RMB", Toast.LENGTH_SHORT).show();
        }

      
        //获取数据更新界面
        handler = new Handler(){

			private int ret, i;
			private String name          ;


			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				
				//当有子线程发送消息过来,就会自动执行
				if(msg.what ==2){
					
					Bundle bundle = msg.getData();
					recv_Mag = bundle.getString("Recv_Mag").toString();
		
	//				Log.i("shen","recv_Mag:"+recv_Mag);
				   
					editor.putString("name", recv_Mag);
					boolean isSuccess = editor.commit();//提交

	//				Toast.makeText(MainActivity.this, recv_Mag  +"RMB", 1000).show();
			
					tv_money.setText(recv_Mag);
				
			  }
			}
        };
 
    }


	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		
		Toast.makeText(MainActivity.this, "校园介绍", 1000).show();
        //跳转到扫码付款
        Intent inten = new Intent(MainActivity.this, SchoolActivity.class);
        startActivity(inten);
		
	}
	
	//读取线程+handler
	class ReadThread extends Thread{
		
		private String str;

		@Override
		public void run() {
			// TODO Auto-generated method stub
			
	        while(flag)
	        {
        	
	        	 //延时函数
	        	 try {
					ReadThread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	        	
	        	 str = ss.Recv_From_Serv();  

	        	 Log.e("Recv_From_Serv", str);
	        	
//	    		Message msg = new Message();
	    		Message msg = Message.obtain();//当内存中有Message对象,可以返回,没有就new Message(),可节省资源
	    		msg.what = 2;//标识码
	        	
	        	Bundle bunlde = new Bundle();
	    		bunlde.putString("Recv_Mag", str);
	    		msg.setData(bunlde);
	    		handler.sendMessage(msg);
	        }
		}
	}
	
	
	
}
