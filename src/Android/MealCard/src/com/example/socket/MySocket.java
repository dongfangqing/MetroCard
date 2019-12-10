package com.example.socket;

import java.net.Socket;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.nio.channels.SocketChannel;

import android.util.Log;

public class MySocket {
	private static Socket ser_Socket;//服务器的Socket 保证Socket不变
	private String NowIP;//保存连接IP port
	private int NowPort;
	private int connectStat= -1;//表示没有操作   连接状态 1表示连接成功  0表示未连接成功
	private int dataReady=0;//接收数据是否完成
	private String DataBuff="";//数据缓冲区
	public int Connect(final String ip,final int port)//连接到指定服务器
	{
		connectStat=-1;//初始化
		//建立连接，成功返回true 失败返回false
		 new Thread(new Runnable(){//使用多线程异步连接防止出错
             @Override
             public void run() {
           		try{
           		NowIP=ip;
           		NowPort=port;
           		
         		ser_Socket=new Socket(ip,port);
               	}catch(Exception ioe){
               		String msgstr = "连接IP异常:" + ioe.toString() + ioe.getMessage() + "\n";//消息换行
               		Log.i("connect",msgstr);
               		connectStat=0;//连接失败
               		return ;
               	}
           		connectStat=1;//连接成功
           		Log.i("IP+PORT",ip+":"+port);
             }
		  }).start();
		
		 //等待连接完成，然后返回结果
		 while(isConnect() == -1);//等待连接完成时时返回
		 
		return isConnect();//返回连接的状态
	}
	//返回已经建立好的socket连接
	public Socket getSocket()
	{
		return ser_Socket;
	}
	public int isConnect()
	{
		return connectStat;
	}
	///////////////////////////////////////
	public String Recv_From_Serv()//接收一次数据
	{
           InputStream is;

			try {
				is = ser_Socket.getInputStream();
				byte[] buffer=new byte[4096];
				is.read(buffer);
				DataBuff = new String(buffer,"GB2312").trim();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	
		return DataBuff;

	}
	
	public void Send_To_Serv(final String cmd,final String data)//发送函数
	{
		new Thread(new Runnable(){//网络都使用多线程异步连接防止出错
            @Override
            public void run() {
            	if(data==null) return;
            	DataOutputStream dos=null;
            	String data1=cmd+data;
            	byte[] BUFF=null;
				try {
					BUFF= data1.getBytes("utf-8");
				} catch (UnsupportedEncodingException e) {}
				
            	//发送
            	try{
              		dos=new DataOutputStream(ser_Socket.getOutputStream());
              		dos.write(BUFF);		
                  }catch(Exception ioe){
                  	String msgstr = "连接IP异常:" + ioe.toString() + ioe.getMessage() + "\n";//消息换行
                  	return ;
                     }
            	return;
            	}
		  }).start();
		return;
	}
	public void Send_To_Serv(final byte[] data)//发送函数
	{
		new Thread(new Runnable(){//网络都使用多线程异步连接防止出错
            @Override
            public void run() {
            	if(data==null) return;
            	DataOutputStream dos=null;
            	byte[] BUFF=data;
				
            	//发送
            	try{
              		dos=new DataOutputStream(ser_Socket.getOutputStream());
              		dos.write(BUFF);		
                  }catch(Exception ioe){
                  	String msgstr = "连接IP异常:" + ioe.toString() + ioe.getMessage() + "\n";//消息换行
                  	return ;
                     }
            	return;
            	}
		  }).start();
		return;
	}
	
	public void close()//关闭套接字
	{
		try{
		ser_Socket.close();
		}catch(Exception ep){return;}
	}
	
}
