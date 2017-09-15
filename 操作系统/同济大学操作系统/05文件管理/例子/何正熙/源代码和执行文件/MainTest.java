/*
 * Created on 2004-4-29
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.JOptionPane;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class MainTest {

	public static void main(String[] args) throws IOException {
		
		FileSystem diskManage = new FileSystem();
		
		DataInputStream in =
						new DataInputStream(
							new BufferedInputStream(
								new FileInputStream("disk.txt")));

					int length = 0;
					length = diskManage.diskInfo.length;
					for (int i = 0; i < length; i++)
					{
						diskManage.diskInfo[i] = in.readChar();
					}

					length = diskManage.rootManageTable.length;
					for (int i = 0; i < length; i++)
					{
						for (int j = 0; j < 16; j++)
						{
							diskManage.rootManageTable[i][j] = in.readChar();
						}

					}

					length = diskManage.fat.length;
					for (int i = 0; i < length; i++)
					{
						diskManage.fat[i] = in.readChar();
					}

					length = diskManage.datafield.length;
					for (int j = 0; j < length-1; j++)
					{
						for (int i = 0; i < 512; i++)
						{
							diskManage.datafield[j][i] = in.readChar();
						}
					}
					in.close();
		
		ArrayList path = new ArrayList();
		
		diskManage.format();
		
		System.out.println("At the beginning, the content in root directory is:");
		ArrayList directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
		System.out.println("Step 1: we create a directory named victor in the root");
		diskManage.createRootItem("victor",2);
		System.out.println();
		
		System.out.println("Step 2: we create a file also named victor in the root");
		diskManage.createRootItem("victor",1);
		System.out.println();
		
		System.out.println("Step 3: we check the content in the root");
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
		System.out.println("Step 4: we try to write something to the file victor");
		String content = JOptionPane.showInputDialog("请输入想写入文件的内容");
		diskManage.saveFile(path,"victor",content);
		StringBuffer loadContent = diskManage.loadFile(path,"victor");
		System.out.println("The content in file victor now is ");
		System.out.println(loadContent);
		System.out.println();
		
		System.out.println("Step 5: Now, we enter the directory victor, and we check the content in victor");
		path.add("victor"); // 进入目录victor
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
		System.out.println("Step 6: Now, we create a new file in directory victor");
		String fileName = JOptionPane.showInputDialog("请输入想创建的文件名字");
		diskManage.createItem(path,fileName,1);
		System.out.println("then the content in directory victor is");
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
		System.out.println("Step 7: Again, write something in the file you created");
		content = JOptionPane.showInputDialog("请输入想写入文件的内容");
		diskManage.saveFile(path,fileName,content);
		loadContent = diskManage.loadFile(path,fileName);
		System.out.println("The content in file " + fileName + " now is ");
		System.out.println(loadContent);
		System.out.println();
		
		System.out.println("Step 8: Now, we delete the file you just created");
		diskManage.deleteItem(path,fileName,1);
		System.out.println("file " + fileName + " deleted!");
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();

		System.out.println("Step 9: we turn back to the root directory.");
		path.remove(path.size()-1);
		System.out.println(" 	    and we can see the content in root is:");
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
		System.out.println("Step 10: finally, we format the disk!!!");
		System.out.println("         and we can see nothing in the disk anymore.");
		diskManage.format();
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		System.out.println();
		
	//	System.exit(1);
/*	
		
		
		diskManage.deleteItem(path,"queen",1);
		directoryContent = diskManage.getDirectoryContent(path);
		System.out.println(directoryContent);
		
		diskManage.deleteItem(path,"queen",1);
		loadContent = diskManage.loadFile(path,"queen");
		System.out.println(loadContent);
		
		DataOutputStream out =
								new DataOutputStream(
									new BufferedOutputStream(
										new FileOutputStream("disk.txt")));

							length = diskManage.diskInfo.length;
							System.out.println("length is " + length);
							
							for (int i = 0; i < length; i++)
							{
								out.writeChar(diskManage.diskInfo[i]);
							}

							length = diskManage.rootManageTable.length;
							for (int i = 0; i < length; i++)
							{
								for (int j = 0; j < 16; j++)
								{
									out.writeChar(diskManage.rootManageTable[i][j]);
								}

							}

							length = diskManage.fat.length;
							for (int i = 0; i < length; i++)
							{
								out.writeChar(diskManage.fat[i]);
							}

							length = diskManage.datafield.length;
							for (int j = 0; j < length; j++)
							{
								for (int i = 0; i < 512; i++)
								{
									out.writeChar(diskManage.datafield[j][i]);
								}
							}

							String fat = String.valueOf(diskManage.fat, 0, 4 * 512);
							out.writeBytes(fat);
							out.close();*/
	}
	
	private static int test()
	{
		return 2000;
	}
}