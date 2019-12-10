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
	private static Socket ser_Socket;//��������Socket ��֤Socket����
	private String NowIP;//��������IP port
	private int NowPort;
	private int connectStat= -1;//��ʾû�в���   ����״̬ 1��ʾ���ӳɹ�  0��ʾδ���ӳɹ�
	private int dataReady=0;//���������Ƿ����
	private String DataBuff="";//���ݻ�����
	public int Connect(final String ip,final int port)//���ӵ�ָ��������
	{
		connectStat=-1;//��ʼ��
		//�������ӣ��ɹ�����true ʧ�ܷ���false
		 new Thread(new Runnable(){//ʹ�ö��߳��첽���ӷ�ֹ����
             @Override
             public void run() {
           		try{
           		NowIP=ip;
           		NowPort=port;
           		
         		ser_Socket=new Socket(ip,port);
               	}catch(Exception ioe){
               		String msgstr = "����IP�쳣:" + ioe.toString() + ioe.getMessage() + "\n";//��Ϣ����
               		Log.i("connect",msgstr);
               		connectStat=0;//����ʧ��
               		return ;
               	}
           		connectStat=1;//���ӳɹ�
           		Log.i("IP+PORT",ip+":"+port);
             }
		  }).start();
		
		 //�ȴ�������ɣ�Ȼ�󷵻ؽ��
		 while(isConnect() == -1);//�ȴ��������ʱʱ����
		 
		return isConnect();//�������ӵ�״̬
	}
	//�����Ѿ������õ�socket����
	public Socket getSocket()
	{
		return ser_Socket;
	}
	public int isConnect()
	{
		return connectStat;
	}
	///////////////////////////////////////
	public String Recv_From_Serv()//����һ������
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
	
	public void Send_To_Serv(final String cmd,final String data)//���ͺ���
	{
		new Thread(new Runnable(){//���綼ʹ�ö��߳��첽���ӷ�ֹ����
            @Override
            public void run() {
            	if(data==null) return;
            	DataOutputStream dos=null;
            	String data1=cmd+data;
            	byte[] BUFF=null;
				try {
					BUFF= data1.getBytes("utf-8");
				} catch (UnsupportedEncodingException e) {}
				
            	//����
            	try{
              		dos=new DataOutputStream(ser_Socket.getOutputStream());
              		dos.write(BUFF);		
                  }catch(Exception ioe){
                  	String msgstr = "����IP�쳣:" + ioe.toString() + ioe.getMessage() + "\n";//��Ϣ����
                  	return ;
                     }
            	return;
            	}
		  }).start();
		return;
	}
	public void Send_To_Serv(final byte[] data)//���ͺ���
	{
		new Thread(new Runnable(){//���綼ʹ�ö��߳��첽���ӷ�ֹ����
            @Override
            public void run() {
            	if(data==null) return;
            	DataOutputStream dos=null;
            	byte[] BUFF=data;
				
            	//����
            	try{
              		dos=new DataOutputStream(ser_Socket.getOutputStream());
              		dos.write(BUFF);		
                  }catch(Exception ioe){
                  	String msgstr = "����IP�쳣:" + ioe.toString() + ioe.getMessage() + "\n";//��Ϣ����
                  	return ;
                     }
            	return;
            	}
		  }).start();
		return;
	}
	
	public void close()//�ر��׽���
	{
		try{
		ser_Socket.close();
		}catch(Exception ep){return;}
	}
	
}
