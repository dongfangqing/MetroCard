package com.example.mealcard;

import java.net.Socket;

import com.example.socket.MySocket;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

public class IPSetActivity extends Activity implements OnClickListener{
	
	private String str = null;
	private EditText ed_ip = null;
	private EditText ed_port = null;
	
	private String Socket_IP = null;//保存连接IP port
	private String Str_Port = null;
	private int Socket_Port = 0;
	private byte[] bs = new byte[]{1,2,3,4,5};
	private MySocket ss;
	private Button btn;

	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setip);
        
         ss = new MySocket();
        
        ed_ip = (EditText) findViewById(R.id.et_ip);
        ed_port = (EditText) findViewById(R.id.et_port);
        
        
        btn = (Button) findViewById(R.id.bt_connect);
        btn.setOnClickListener(this);
            
    
    }

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		Socket mmSocket;
		switch (v.getId()) {
		case R.id.bt_connect:
			
		  	
			  	Socket_IP = ed_ip.getText().toString().trim();                //获取EditText里面的值
			  	Str_Port = ed_port.getText().toString().trim();

			    
			    if(Socket_IP.equals("") || Str_Port.equals(""))
			    {
			    	Toast.makeText(IPSetActivity.this, "请输入端口号", Toast.LENGTH_LONG).show();
			    }
			    
			    else {
			    	//把字符串转换成int
				    Socket_Port=Integer.parseInt(Str_Port);
//			    	//进行网络连接
			        ss.Connect(Socket_IP, Socket_Port);	

			        //跳转到主页面
				    Intent intent = new Intent(IPSetActivity.this, MainActivity.class);
				    startActivity(intent);	
				}

		}
		
	}
	
}




