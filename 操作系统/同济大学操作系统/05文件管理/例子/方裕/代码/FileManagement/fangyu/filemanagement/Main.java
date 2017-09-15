/*
 * Created on 2004-4-30
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package fangyu.filemanagement;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class Main {

	public static void main(String[] args) {
		MainFrame frame = new MainFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.show();
	}
}

class MainFrame extends JFrame {
	public static final int DEFAULT_WIDTH = 635;
	public static final int DEFAULT_HEIGHT = 500;

	public MainFrame() {
		setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		Container contentPane = getContentPane();
		MainPanel panel = new MainPanel();
		contentPane.add(panel);
		setTitle("File Management");
		setResizable(false);
		setLocation(100, 100);
	}

	class MainPanel extends JPanel {
		private Disc disc;
		public String path = new String("$");
		private int CurBlock;
		private JTextField textCommand;
		private ArrayList CurDirs = new ArrayList();
		private ArrayList CurFiles = new ArrayList();
		private JComboBox CombDir;
		private JComboBox CombFile;
		private JTextArea DirInfo;
		private JTextArea FileInfo;

		public MainPanel() {
			setLayout(null);
			//当前目录
			JLabel label1 = new JLabel("当前目录");
			label1.setSize(100, 20);
			label1.setLocation(10, 10);
			add(label1);
			textCommand = new JTextField(path, 50);
			textCommand.setSize(200, 20);
			textCommand.setLocation(10, 30);
			textCommand.setEditable(false);
			add(textCommand);

			JButton exit = new JButton("退出系统");
			exit.addActionListener(new ExitSysListener());
			exit.setSize(100, 25);
			exit.setLocation(230, 30);
			add(exit);

			//当前目录下子目录
			JLabel label2 = new JLabel("文件夹");
			label2.setSize(100, 20);
			label2.setLocation(10, 110);
			add(label2);
			CombDir = new JComboBox();
			CombDir.addActionListener(new SelectDirListener());
			CombDir.setSize(200, 20);
			CombDir.setLocation(10, 130);
			add(CombDir);
			//当前目录下的文件
			JLabel label3 = new JLabel("文件");
			label3.setSize(100, 20);
			label3.setLocation(350, 110);
			add(label3);

			CombFile = new JComboBox();
			CombFile.addActionListener(new SelectFileListener());
			CombFile.setSize(200, 20);
			CombFile.setLocation(350, 130);
			add(CombFile);
			//文件夹操作按钮
			JButton CreateDir = new JButton("新建");
			CreateDir.addActionListener(new CreateDirListener());
			CreateDir.setSize(60, 30);
			CreateDir.setLocation(10, 70);
			add(CreateDir);

			JButton DelDir = new JButton("删除");
			DelDir.addActionListener(new DelDirListener());
			DelDir.setSize(60, 30);
			DelDir.setLocation(80, 70);
			add(DelDir);

			JButton EnterDir = new JButton("进入");
			EnterDir.addActionListener(new EnterListener());
			EnterDir.setSize(60, 30);
			EnterDir.setLocation(150, 70);
			add(EnterDir);

			JButton ExitDir = new JButton("上级");
			ExitDir.addActionListener(new ExitListener());
			ExitDir.setSize(60, 30);
			ExitDir.setLocation(220, 70);
			add(ExitDir);

			//文件操作按钮
			JButton CreateFile = new JButton("新建");
			CreateFile.addActionListener(new CreateFileListener());
			CreateFile.setSize(60, 30);
			CreateFile.setLocation(350, 70);
			add(CreateFile);

			JButton DelFile = new JButton("删除");
			DelFile.addActionListener(new DelFileListener());
			DelFile.setSize(60, 30);
			DelFile.setLocation(420, 70);
			add(DelFile);

			//文件夹信息显示
			DirInfo = new JTextArea(8, 40);
			DirInfo.setLineWrap(true);
			DirInfo.setEditable(false);
			DirInfo.setSize(200, 250);
			DirInfo.setLocation(10, 180);
			add(DirInfo);

			//文件信息显示
			FileInfo = new JTextArea(8, 40);
			FileInfo.setLineWrap(true);
			FileInfo.setSize(200, 250);
			FileInfo.setLocation(350, 180);
			add(FileInfo);
			JButton SaveFile = new JButton("保存");
			SaveFile.addActionListener(new SaveFileListener());
			SaveFile.setSize(60, 30);
			SaveFile.setLocation(490, 70);
			add(SaveFile);
			JButton Reset = new JButton("重写");
			Reset.addActionListener(new ResetListener());
			Reset.setSize(60, 30);
			Reset.setLocation(560, 70);
			add(Reset);
			
			
			try {
				ObjectInputStream in =
					new ObjectInputStream(new FileInputStream("filesystem.fy"));
				disc = (Disc) in.readObject();
				in.close();
				disc.reload(CurDirs, CurFiles);
				for(int i = 0; i < CurDirs.size(); i++)
				{
					CombDir.addItem(CurDirs.get(i));
				}
				for(int i = 0; i < CurFiles.size(); i++)
				{
					CombFile.addItem(CurFiles.get(i));
				}
				disc.printDisc();
				if(disc == null)
				{
					disc = new Disc();
				}
			} catch (IOException err) {
				disc = new Disc();
				System.out.println(err.getMessage());
			} catch (ClassNotFoundException err) {
				System.out.println(err.getMessage());
			}
	}

		class CreateDirListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String DirName = JOptionPane.showInputDialog(null, "请输入文件夹名");
				if (DirName == null) {
					return;
				} else {
					DirName = DirName.trim();
					if (DirName.length() > 16) {
						JOptionPane.showMessageDialog(null, "文件夹名应小于16个字符");
						return;
					}
				}

				if (DirName.equals("")) {
					//判断是否为空
					JOptionPane.showMessageDialog(null, "请输入文件夹名");
					return;
				} else {
					//判断是否重复
					for (int i = 0; i < CurDirs.size(); ++i) {
						if (DirName.equals(CurDirs.get(i))) {
							JOptionPane.showMessageDialog(null, "该文件夹已存在");
							return;
						}
					}

					if (disc.addDirectory(CurBlock, DirName)) {
						CurDirs.add(DirName);
						CombDir.addItem(DirName);
						//						disc.printDisc();
					} else {
					}
				}
			}
		}

		class CreateFileListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String FileName = JOptionPane.showInputDialog(null, "请输入文件名");
				if (FileName == null) {
					return;
				} else {
					FileName = FileName.trim();
					if (FileName.length() > 16) {
						JOptionPane.showMessageDialog(null, "文件名应小于16个字符");
						return;
					}
				}

				if (FileName.equals("")) {
					//判断是否为空
					JOptionPane.showMessageDialog(null, "请输入文件名");
					return;
				} else {
					//判断是否重复
					for (int i = 0; i < CurFiles.size(); ++i) {
						if (FileName.equals(CurFiles.get(i))) {
							JOptionPane.showMessageDialog(null, "该文件已存在");
							return;
						}
					}

					if (disc.addFile(CurBlock, FileName)) {
						CombFile.addItem(FileName);
						CurFiles.add(FileName);

						//编辑文件
						//						disc.printDisc();
					} else {
					}
				}
			}
		}

		class SelectFileListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String FileName = (String) CombFile.getSelectedItem();
				if (FileName != null) {
					String Text = disc.getFileContent(CurBlock, FileName);
					FileInfo.setText(Text);

					String FileText = "------------------------\n";
					FileText += "文件位置："
						+ disc.getLocation(CurBlock, FileName)
						+ "\n";
					FileText += "文件大小："
						+ disc.getSize(CurBlock, FileName)
						+ "\n";
					FileText += "------------------------\n";
					DirInfo.setText(FileText);
				}
			}
		}

		class SelectDirListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String DirName = (String) CombDir.getSelectedItem();
				if (DirName != null) {
					String DirText = "------------------------\n";
					DirText += "文件夹位置："
						+ disc.getLocation(CurBlock, DirName)
						+ "\n";
					DirText += "------------------------\n";
					DirInfo.setText(DirText);
				}
			}
		}

		class SaveFileListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String FileName = (String) CombFile.getSelectedItem();
				String Text = FileInfo.getText().trim();
				disc.saveFileContent(CurBlock, FileName, Text);

				String FileText = "------------------------\n";
				FileText += "文件位置："
					+ disc.getLocation(CurBlock, FileName)
					+ "\n";
				FileText += "文件大小：" + disc.getSize(CurBlock, FileName) + "\n";
				FileText += "------------------------\n";
				DirInfo.setText(FileText);
			}
		}

		class EnterListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String DirName = (String) CombDir.getSelectedItem();
				if (DirName == null || DirName.equals("")) {
					JOptionPane.showMessageDialog(null, "请选择文件夹");
					return;
				}
				path = path + "\\" + DirName;
				CurBlock =
					disc.enterDirectory(CurBlock, DirName, CurDirs, CurFiles);
				textCommand.setText(path);
				CombDir.removeAllItems();
				for (int i = 0; i < CurDirs.size(); i++) {
					CombDir.addItem((String) CurDirs.get(i));
				}
				CombFile.removeAllItems();
				for (int i = 0; i < CurFiles.size(); i++) {
					CombFile.addItem((String) CurFiles.get(i));
				}
				DirInfo.setText("");
				FileInfo.setText("");
			}
		}

		class ExitListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				int newBlock = disc.exitDirectory(CurBlock, CurDirs, CurFiles);
				if (newBlock == -1) {
					return;
				}
				CurBlock = newBlock;
				int j = path.lastIndexOf("\\");
				path = path.substring(0, j);
				textCommand.setText(path);
				CombDir.removeAllItems();
				FileInfo.setText("");
				DirInfo.setText("");
				for (int i = 0; i < CurDirs.size(); i++) {
					CombDir.addItem((String) CurDirs.get(i));
				}
				CombFile.removeAllItems();
				for (int i = 0; i < CurFiles.size(); i++) {
					CombFile.addItem((String) CurFiles.get(i));
				}
			}
		}

		class DelFileListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String FileName = (String) CombFile.getSelectedItem();
				if (FileName != null && !FileName.equals("")) {
					FileInfo.setText("");
					disc.delFile(CurBlock, FileName);
					CurFiles.remove(FileName);
					CombFile.removeItem(FileName);
				}
			}
		}

		class DelDirListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				String DirName = (String) CombDir.getSelectedItem();
				if (DirName != null && !DirName.equals("")) {
					disc.delDirectory(CurBlock, DirName);
					DirInfo.setText("");
					FileInfo.setText("");
					CurDirs.remove(DirName);
					CombDir.removeItem(DirName);
					//					disc.printDisc();
					//					disc.printFAT();
				}
			}
		}

		class ResetListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				FileInfo.setText("");
			}
		}

		class ExitSysListener implements ActionListener {
			public void actionPerformed(ActionEvent event) {
				try {
					//File f = new File("filesystem.fy");
					//f.createNewFile();
					ObjectOutputStream out =
						new ObjectOutputStream(
							new FileOutputStream("filesystem.fy"));
					out.writeObject(disc);
					out.close();
					System.exit(0);
				} catch (IOException err) {
					System.out.println(err.getMessage());
				}
			}
		}
	}
}
