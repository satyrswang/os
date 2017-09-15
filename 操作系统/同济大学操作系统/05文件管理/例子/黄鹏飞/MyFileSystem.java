import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

class Console {
	// Create a title string from the class name:
	public static String title(Object o) {
		String t = o.getClass().toString();
		// Remove the word "class":
		if (t.indexOf("class") != -1)
			t = t.substring(6);
		return t;
	}

	public static void setupClosing(JFrame frame) {
		// The JDK 1.2 Solution as an
		// anonymous inner class:
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
		// The improved solution in JDK 1.3:
		// frame.setDefaultCloseOperation(
		//     EXIT_ON_CLOSE);
	}

	public static void run(JFrame frame) {
		setupClosing(frame);
		frame.pack();
		frame.setVisible(true);
	}

	public static void run(JFrame frame, int width, int height) {
		setupClosing(frame);
		frame.setSize(width, height);
		frame.pack();
		frame.setVisible(true);
	}

	public static void run(JPanel panel, int width, int height) {
		JFrame frame = new JFrame(title(panel));
		setupClosing(frame);
		frame.getContentPane().add(panel);
		frame.setSize(width, height);
		frame.pack(); //*
		frame.setVisible(true);
	}
}

class SystemManager {
	public MyFileSystemPanel p = null;

	// 用char（JAVA中是占两个byte）来模拟Byte。
	final static int cSector = 512; // 512 Byte

	public char[][] rootDirectory = new char[cSector / 16][16];
	//根目录（目录）占1个Sector
	//有32条记录，每条记录占16个char
	public char[] fatTable = new char[cSector * 4]; // FAT占4个扇区
	public char[][] dataArea = new char[cSector * 4][cSector];
	//dataArea占2048个扇区

	final static int maxItem = 32; //  cSector/16=32，目录里面最大的项数
	final static int itemLength = 16; //  一个目录项或文件描述表的长度
	final static char isTail_FAT = 0xffff; //  用的时候当成int用，(int)
	final static char isFile = 1;
	final static char isFolder = 0;
	final static char isUsable_RCD = (char) - 1;
	final static int stateIndex = 11;
	final static int fatIndex = 12;
	final static int szIndex = 13;
	final static int nameLengthIndex = 15;

	SystemManager() {
		//format();
	}

	public void format() {
		boolean isSuccessful = true;
		for (int i = 0; i < fatTable.length; i++) {
			//将fat表的所有项初始化为指向自己，
			//代表该块为Free。
			setFree_Block(i);
			isSuccessful = this.isFree_Block(i);
			//System.out.println("1"+this.isFree_Block(i));
		}

		for (int i = 0; i < rootDirectory.length; i++) {
			this.setUsable_RCD(rootDirectory[i]);
			//System.out.println("2"+this.isUsable_RCD(rootDirectory[i]));
			isSuccessful = isUsable_RCD(rootDirectory[i]);
		}

		if (isSuccessful)
			System.out.println("Formatting is successful.");
		else
			System.out.println("Formatting is fail.");
	}

	public boolean isFree_Block(int indexBlock) {
		if (indexBlock < cSector * 4
			&& indexBlock >= 0
			&& fatTable[indexBlock] == indexBlock)
			return true;
		return false;
	}

	public boolean isUsable_RCD(char[] d) {
		if (stateIndex < d.length
			&& d[stateIndex] == SystemManager.isUsable_RCD)
			return true;
		return false;
	}

	public void setFree_Block(int indexBlock) {
		if (indexBlock < cSector * 4 && indexBlock >= 0)
			fatTable[indexBlock] = (char) indexBlock;
	}

	public void setUsable_RCD(char[] d) {
		//System.out.println("setUsable_RCD(char[] d)");
		//System.out.println("stateIndex < d.length: "+(stateIndex <
		// d.length));
		//System.out.println("stateIndex : "+stateIndex + "d[stateIndex]");
		if (stateIndex < d.length)
			d[stateIndex] = SystemManager.isUsable_RCD;
	}

	public char getState_RCD(char[] d) {
		return d[stateIndex];
	}

	public void setFatIndex_RCD(char[] d, int i) {
		if (stateIndex < d.length)
			d[fatIndex] = (char) i;
	}

	public char getFatIndex_RCD(char[] d) {
		return d[fatIndex];
	}

	public void setSize_RCD(char[] d, int sz) {
		if (stateIndex < d.length)
			d[szIndex] = (char) sz;
	}

	public boolean isGoodName(String name) {
		if (name.length() > 11 || name.length() <= 0)
			return false;
		else if (name.indexOf('/') != -1)
			return false;
		else if (name.indexOf('\\') != -1)
			return false;
		else if (name.indexOf(':') != -1)
			return false;
		else if (name.indexOf('*') != -1)
			return false;
		else if (name.indexOf('?') != -1)
			return false;
		else if (name.indexOf('"') != -1)
			return false;
		else if (name.indexOf('<') != -1)
			return false;
		else if (name.indexOf('<') != -1)
			return false;
		else if (name.indexOf('|') != -1)
			return false;
		else if (name.indexOf('.') != -1)
			return false;

		return true;
	}

	void setName_RCD(char[] d, String name) {
		if (isGoodName(name) && name.length() <= d.length) {
			d[nameLengthIndex] = (char) name.length();
			int i;
			for (i = 0; i < name.length(); i++) {
				d[i] = name.charAt(i);
			}
		}
	}

	String getName(char[] rcd) {
		String name = "";
		int i;
		for (i = 0; i < rcd[nameLengthIndex]; i++) {
			name += rcd[i];
		}
		return name;
	}

	void setTail_FAT(int index) {
		fatTable[index] = isTail_FAT;
	}

	void setNext_FAT(int index, int next) {
		fatTable[index] = (char) next;
	}

	ArrayList findAllBlock(char[] rcd) {
		if (isUsable_RCD(rcd))
			return null;

		ArrayList l = new ArrayList();
		if (getState_RCD(rcd) == isFolder) {
			l.add(new Character(getFatIndex_RCD(rcd)));
			return l;
		}

		// is file
		char index = getFatIndex_RCD(rcd);

		while (index != isTail_FAT) {
			l.add(new Character(index));
			index = fatTable[index];
		}

		return l;
	}

	public String dir(char[][] folder) {
		String text = "";
		int i;
		for (i = 0; i < folder.length; i++) {
			//System.out.println(
			//	""
			//		+ (int) getState_RCD(folder[i])
			//		+ " == "
			//		+ (int) isUsable_RCD
			//		+ "?");
			if (getState_RCD(folder[i]) == isUsable_RCD) {
				//	text+=i+" FREE!!";
			} else if (getState_RCD(folder[i]) == isFile) {
				String s;
				s = getName(folder[i]);
				int j;
				for (j = s.length(); j < 15; j++) {
					s += ' ';
				}
				s += "<FILE>" + "\n";
				text += s;
			} else if (getState_RCD(folder[i]) == isFolder) {
				String s;
				s = getName(folder[i]);
				int j;
				for (j = s.length(); j < 15; j++) {
					s += ' ';
				}
				s += "<DIR>" + "\n";
				text += s;
			}
		}

		text += "               <DIR>          .\n"
			+ "               <DIR>         ..";
		return text;
	}

	boolean setRCD_to_Folder(char[][] folder, String name) {
		printSectorInfo(folder);

		System.out.println("setRCD_to_Folder" + (folder == rootDirectory));
		System.out.println("mkdir --> " + name);
		int i;
		if (isGoodName(name) == false)
			return false;

		int usableRCDIndex = -1;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0)
				return false;

			if (getState_RCD(folder[i]) == isUsable_RCD)
				usableRCDIndex = i;
		}
		if (usableRCDIndex == -1) //没有可用的记录，超过32项了。
			return false;

		int freeBlockIndex = -1;
		for (i = 0; i < cSector * 4; i++) {
			if (isFree_Block(i)) {
				freeBlockIndex = i;
				break;
			}
		}
		if (freeBlockIndex == -1)
			return false;

		folder[usableRCDIndex][stateIndex] = isFolder;
		folder[usableRCDIndex][nameLengthIndex] = (char) name.length();
		System.out.println(
			"Test!!: "
				+ nameLengthIndex
				+ ","
				+ (int) rootDirectory[usableRCDIndex][nameLengthIndex]
				+ ","
				+ (int) folder[usableRCDIndex][nameLengthIndex]);
		for (i = 0; i < name.length(); i++) {
			folder[usableRCDIndex][i] = name.charAt(i);
			System.out.println(
				"Test!!: "
					+ usableRCDIndex
					+ ","
					+ (char) rootDirectory[usableRCDIndex][i]
					+ ","
					+ (char) folder[usableRCDIndex][i]);
		}
		folder[usableRCDIndex][fatIndex] = (char) freeBlockIndex;
		setNext_FAT(freeBlockIndex, isTail_FAT); // 设置fat表中块已经被占用。

		System.out.println(cSector / 16 + "");
		for (i = 0; i < (cSector / 16); i++) {
			//System.out.println("freeBlockIndex,cSector,cSector/16*i,stateIndex:"
			//		+freeBlockIndex+","
			//		+cSector+","
			//		+(16*i)+","
			//		+stateIndex+",");
			int index = 16 * i + stateIndex;
			setUsable_RCD(index, dataArea[freeBlockIndex]);
		}
		System.out.println("is Free Block is " + isFree_Block(freeBlockIndex));
		folder[usableRCDIndex][szIndex] = (char) 0;

		printSectorInfo(folder);
		return true;
	}

	private void setUsable_RCD(int i, char[] s) {
		s[i] = SystemManager.isUsable_RCD;
	}

	boolean mkdir(char[][] folder, String name) {
		folder = rootDirectory;
		return setRCD_to_Folder(folder, name);
	}

	boolean mkdir(int indexBlock, char[][] folder, String name) {
		System.out.println("mkdir,indexBlock: " + indexBlock);
		return setRCD_to_Folder(indexBlock, folder, name);
	}

	public void printSectorInfo(char[] s) {
		int i, j;
		for (i = 0; i < cSector / 16; i++) {
			for (j = 0; j < 16; j++) {
				System.out.print((int) s[i * 16 + j] + ",");
			}
			System.out.println("");
		}
	}

	public void printSectorInfo(char[][] t) {
		int i, j;
		for (i = 0; i < cSector / 16; i++) {
			for (j = 0; j < 16; j++) {
				System.out.print((int) t[i][j] + ",");
			}
			System.out.println("");
		}
	}

	private boolean setRCD_to_Folder(
		int indexBlock,
		char[][] folder,
		String name) {
		System.out.println("mkdir --> " + name);
		int i;
		if (isGoodName(name) == false)
			return false;

		int usableRCDIndex = -1;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0)
				return false;

			if (getState_RCD(folder[i]) == isUsable_RCD)
				usableRCDIndex = i;
		}
		if (usableRCDIndex == -1) //没有可用的记录，超过32项了。
			return false;

		int freeBlockIndex = -1;
		for (i = 0; i < cSector * 4; i++) {
			if (isFree_Block(i)) {
				freeBlockIndex = i;
				break;
			}
		}
		if (freeBlockIndex == -1)
			return false;

		//folder[usableRCDIndex][stateIndex] = isFolder;
		dataArea[indexBlock][usableRCDIndex * 16 + stateIndex] = isFolder;
		//folder[usableRCDIndex][nameLengthIndex] = (char) name.length();
		dataArea[indexBlock][usableRCDIndex * 16 + nameLengthIndex] =
			(char) name.length();
		for (i = 0; i < name.length(); i++) {
			dataArea[indexBlock][usableRCDIndex * 16 + i] = name.charAt(i);
			//folder[usableRCDIndex][i] = name.charAt(i);
		}
		//folder[usableRCDIndex][fatIndex] = (char) freeBlockIndex;
		dataArea[indexBlock][usableRCDIndex * 16 + fatIndex] =
			(char) freeBlockIndex;
		setNext_FAT(freeBlockIndex, isTail_FAT); // 设置fat表中块已经被占用。

		System.out.println(cSector / 16 + "");
		for (i = 0; i < (cSector / 16); i++) {
			//System.out.println("freeBlockIndex,cSector,cSector/16*i,stateIndex:"
			//		+freeBlockIndex+","
			//		+cSector+","
			//		+(16*i)+","
			//		+stateIndex+",");
			int index = 16 * i + stateIndex;
			setUsable_RCD(index, dataArea[freeBlockIndex]);
		}
		System.out.println("is Free Block is " + isFree_Block(freeBlockIndex));
		//folder[usableRCDIndex][szIndex] = (char) 0;
		dataArea[indexBlock][usableRCDIndex * 16 + szIndex] = (char) 0;

		return true;
	}

	public boolean newFile(char[][] folder, String name) {
		return setRCD_to_File(folder, name);
	}

	public boolean newFile(char indexDataArea, char[][] folder, String name) {
		return setRCD_to_File(indexDataArea, folder, name);
	}

	private boolean setRCD_to_File(
		char indexDataArea,
		char[][] folder,
		String name) {
		int i;
		if (isGoodName(name) == false)
			return false;
		printSectorInfo(folder);
		printSectorInfo(dataArea[indexDataArea]);

		int usableRCDIndex = -1;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(name) == 0)
				return false;

			if (getState_RCD(folder[i]) == isUsable_RCD)
				usableRCDIndex = i;
		}
		if (usableRCDIndex == -1) //没有可用的记录，超过32项了。
			return false;

		int freeBlockIndex = -1;
		for (i = 0; i < cSector * 4; i++) {
			if (isFree_Block(i)) {
				freeBlockIndex = i;
				break;
			}
		}
		if (freeBlockIndex == -1)
			return false;

		//folder[usableRCDIndex][stateIndex] = isFile;
		dataArea[indexDataArea][usableRCDIndex * 16 + stateIndex] = isFile;
		//folder[usableRCDIndex][nameLengthIndex] = (char) name.length();
		dataArea[indexDataArea][usableRCDIndex * 16 + nameLengthIndex] =
			(char) name.length();
		for (i = 0; i < name.length(); i++) {
			//folder[usableRCDIndex][i] = name.charAt(i);
			dataArea[indexDataArea][usableRCDIndex * 16 + i] = name.charAt(i);
		}
		//folder[usableRCDIndex][fatIndex] = (char) freeBlockIndex;
		dataArea[indexDataArea][usableRCDIndex * 16 + fatIndex] =
			(char) freeBlockIndex;
		for (i = 0; i < 512; i++)
			dataArea[freeBlockIndex][i] = 0;
		setNext_FAT(freeBlockIndex, isTail_FAT);
		System.out.println("" + isFree_Block(freeBlockIndex));
		//folder[usableRCDIndex][szIndex] = (char) 0;
		dataArea[indexDataArea][usableRCDIndex * 16 + szIndex] = (char) 0;

		return true;
	}

	private boolean setRCD_to_File(char[][] folder, String name) {
		int i;
		if (isGoodName(name) == false)
			return false;

		int usableRCDIndex = -1;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(name) == 0)
				return false;

			if (getState_RCD(folder[i]) == isUsable_RCD)
				usableRCDIndex = i;
		}
		if (usableRCDIndex == -1) //没有可用的记录，超过32项了。
			return false;

		int freeBlockIndex = -1;
		for (i = 0; i < cSector * 4; i++) {
			if (isFree_Block(i)) {
				freeBlockIndex = i;
				break;
			}
		}
		if (freeBlockIndex == -1)
			return false;

		folder[usableRCDIndex][stateIndex] = isFile;
		folder[usableRCDIndex][nameLengthIndex] = (char) name.length();
		for (i = 0; i < name.length(); i++)
			folder[usableRCDIndex][i] = name.charAt(i);
		folder[usableRCDIndex][fatIndex] = (char) freeBlockIndex;
		setNext_FAT(freeBlockIndex, isTail_FAT);
		for (i = 0; i < 512; i++)
			dataArea[freeBlockIndex][i] = 0;
		System.out.println("" + isFree_Block(freeBlockIndex));
		folder[usableRCDIndex][szIndex] = (char) 0;

		return true;
	}

	private boolean delFile_setFree_setUsable(char[][] folder, String name) {
		int toDelIndex = -1;
		int i;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(name) == 0) {
				toDelIndex = i;

				//  找到文件，删除文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				while (blockIndex != isTail_FAT) {
					int temp = fatTable[blockIndex];
					setFree_Block(blockIndex);
					blockIndex = temp;
				}

				setUsable_RCD(folder[i]);

				return true;
			}
		}

		return false;
	}

	private boolean delFile_setFree_setUsable(
		char indexDataArea,
		char[][] folder,
		String name) {
		int toDelIndex = -1;
		int i;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(name) == 0) {
				toDelIndex = i;

				//  找到文件，删除文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				while (blockIndex != isTail_FAT) {
					int temp = fatTable[blockIndex];
					setFree_Block(blockIndex);
					blockIndex = temp;
				}

				setUsable_RCD(i * 16 + stateIndex, dataArea[indexDataArea]);
				return true;
			}
		}

		return false;
	}

	public boolean del(char[][] folder, String name) {
		return delFile_setFree_setUsable(folder, name);
	}

	public boolean del(char indexDataArea, char[][] folder, String name) {
		return delFile_setFree_setUsable(indexDataArea, folder, name);
	}

	public boolean cdToFolder(char[][] folder, String name) {
		int i;
		name = name.trim();
		if (name.compareTo(".") == 0)
			return true;
		else if (name.compareTo("..") == 0) {
			if (p.path.size() == 1)
				return false;
			else {
				p.path.remove(p.path.size() - 1);
				p.currentPath.remove(p.currentPath.size() - 1);
				return true;
			}
		} else if (isGoodName(name) == false)
			return false;

		//System.out.println("cd to --> " + name);

		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0) {
				p.path.add(new Character(getFatIndex_RCD(folder[i])));
				//	System.out.println("in cd, getName!! : " +
				// getName(folder[i]));
				p.currentPath.add(new String(getName(folder[i])));
				//	System.out.println("cdcd" + p.path.size());
				return true;
			}
		}
		return false;
	}

	public boolean cdToFolder(int indexBlock, char[][] folder, String name) {
		int i;
		name = name.trim();
		if (name.compareTo(".") == 0)
			return true;
		else if (name.compareTo("..") == 0) {
			if (p.path.size() == 1)
				return false;
			else {
				p.path.remove(p.path.size() - 1);
				p.currentPath.remove(p.currentPath.size() - 1);
				return true;
			}
		} else if (isGoodName(name) == false)
			return false;

		//System.out.println("cd to --> " + name);

		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0) {
				p.path.add(new Character(getFatIndex_RCD(folder[i])));
				//	System.out.println("in cd, getName!! : " +
				// getName(folder[i]));
				p.currentPath.add(new String(getName(folder[i])));
				//	System.out.println("cdcd" + p.path.size());
				return true;
			}
		}
		return false;
	}

	public boolean rmdir(char indexDataArea, char[][] folder, String name) {
		int i, j, k;
		name = name.trim();
		if (isGoodName(name) == false)
			return false;

		boolean canFind = true;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0) {
				char indexBlock = getFatIndex_RCD(folder[i]);
				char[][] temp = new char[32][16];
				for (j = 0; j < 32; j++) {
					for (k = 0; k < 16; k++) {
						temp[j][k] = dataArea[indexBlock][j * 16 + k];
					}
				}

				for (j = 0; j < 32; j++) {
					String n;
					if (this.getState_RCD(temp[j]) == isFolder) {
						n = getName(temp[j]);
						rmdir(indexBlock, temp, n);
					} else if (this.getState_RCD(temp[j]) == isFile) {
						n = getName(temp[j]);
						del(indexBlock, temp, n);
					}
				}

				setFree_Block(indexBlock);
				setUsable_RCD(indexDataArea, folder[i]);
				return true;
			}
		}
		return false;
	}

	public boolean rmdir(char[][] folder, String name) {
		int i, j, k;
		name = name.trim();
		if (isGoodName(name) == false)
			return false;

		boolean canFind = true;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFolder
				&& getName(folder[i]).compareTo(name) == 0) {
				char indexBlock = getFatIndex_RCD(folder[i]);
				char[][] temp = new char[32][16];
				for (j = 0; j < 32; j++) {
					for (k = 0; k < 16; k++) {
						temp[j][k] = dataArea[indexBlock][j * 16 + k];
					}
				}

				for (j = 0; j < 32; j++) {
					String n;
					if (this.getState_RCD(temp[j]) == isFolder) {
						n = getName(temp[j]);
						rmdir(indexBlock, temp, n);
					} else if (this.getState_RCD(temp[j]) == isFile) {
						n = getName(temp[j]);
						del(indexBlock, temp, n);
					}
				}

				setFree_Block(indexBlock);
				setUsable_RCD(folder[i]);
				return true;
			}
		}
		return false;
	}

	String getDiskInfo() {
		String text = "";
		text += getFreeBlockNumber()
			+ " Free Block(s).\n"
			+ getUsedBlockNumber()
			+ " Used Block(s).\n";
		return text;
	}

	int getFreeBlockNumber() {
		int i;
		int n = 0;
		for (i = 0; i < 512 * 4; i++) {
			if (isFree_Block(fatTable[i]))
				n++;
		}
		return n;
	}

	int getUsedBlockNumber() {
		return 512 * 4 - getFreeBlockNumber();
	}

	public boolean readFileToText(
		char[][] folder,
		String filename,
		StringBuffer strBuffer) {
		int toDelIndex = -1;
		int i;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(filename) == 0) {
				toDelIndex = i;
				int size = folder[i][szIndex];

				//  找到文件，准备读入文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				int caret = 0;
				while (blockIndex != isTail_FAT) {
					int temp = fatTable[blockIndex];
					int j;
					//  逐个字符读入
					for (j = 0; j < 512 && caret < size; j++, caret++) {
						strBuffer.append(dataArea[blockIndex][j]);
					}
					blockIndex = temp;
				}
				return true;
			}
		}
		return false;
	}

	public boolean readFileToText(
		char indexDataArea,
		char[][] folder,
		String filename,
		StringBuffer strBuffer) {

		int toDelIndex = -1;
		int i;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(filename) == 0) {
				toDelIndex = i;
				int size = folder[i][szIndex];

				//  找到文件，准备读入文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				int caret = 0;
				while (blockIndex != isTail_FAT) {
					int temp = fatTable[blockIndex];
					int j;
					//  逐个字符读入
					for (j = 0; j < 512 && caret < size; j++, caret++) {
						strBuffer.append(dataArea[blockIndex][j]);
					}
					blockIndex = temp;
				}
				return true;
			}
		}
		return false;
	}

	public boolean writeTextToFile(
		final char[][] folder,
		String filename,
		String text) {
		// 空间不足
		if (getFreeBlockNumber() * 512 + 512 < text.length())
			return false;

		int toDelIndex = -1;
		int i;
		for (i = 0; i < folder.length; i++) {
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(filename) == 0) {
				toDelIndex = i;
				int size = folder[i][szIndex] = (char) text.length();

				//  找到文件，准备写入文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				int caret = 0;
				int j, k;
				j = 0;
				//首块数据
				for (k = 0; k < 512 && caret < size; k++, caret++) {
					dataArea[blockIndex][k] = text.charAt(caret);
				}
				int t;
				for (j = 1;
					j < (t = size / 512 + ((size % 512 == 0) ? 0 : 1));
					j++) {
					int next = isTail_FAT;
					for (k = 0; k < 512 * 4; k++)
						if (isFree_Block(k)) {
							next = k;
							break;
						}

					setNext_FAT(blockIndex, next);
					blockIndex = next;
					fatTable[blockIndex] = isTail_FAT;
					for (k = 0; k < 512 && caret < size; k++, caret++) {
						dataArea[blockIndex][k] = text.charAt(caret);
					}
				}
				fatTable[blockIndex] = isTail_FAT;

				this.printSectorInfo(dataArea[blockIndex]);
				this.printSectorInfo(rootDirectory);
				return true;
			}
		}
		return false;
	}

	public boolean writeTextToFile(
		char indexDataArea,
		char[][] folder,
		String filename,
		String text) {
		// 空间不足
		if (getFreeBlockNumber() * 512 + 512 < text.length())
			return false;

		int toDelIndex = -1;
		int i;
		printSectorInfo(folder);
		for (i = 0; i < folder.length; i++) {
			System.out.println(getName(folder[i]) + "," + filename);
			if (getState_RCD(folder[i]) == isFile
				&& getName(folder[i]).compareTo(filename) == 0) {
				toDelIndex = i;
				int size =
					dataArea[indexDataArea][i * 16 + szIndex] =
						(char) text.length();

				//  找到文件，准备写入文件
				int blockIndex = getFatIndex_RCD(folder[i]);
				System.out.println(
					"" + blockIndex + "," + fatTable[blockIndex]);
				int caret = 0;
				int j, k;
				j = 0;
				//首块数据
				for (k = 0; k < 512 && caret < size; k++, caret++) {
					dataArea[blockIndex][k] = text.charAt(caret);
				}
				int t;
				for (j = 1;
					j < (t = size / 512 + ((size % 512 == 0) ? 0 : 1));
					j++) {
					int next = isTail_FAT;
					for (k = 0; k < 512 * 4; k++)
						if (isFree_Block(k)) {
							next = k;
							break;
						}

					setNext_FAT(blockIndex, next);
					blockIndex = next;
					fatTable[blockIndex] = isTail_FAT;
					for (k = 0; k < 512 && caret < size; k++, caret++) {
						dataArea[blockIndex][k] = text.charAt(caret);
					}
				}
				fatTable[blockIndex] = isTail_FAT;

				this.printSectorInfo(dataArea[blockIndex]);
				//this.printSectorInfo(rootDirectory);
				return true;
			}
		}
		return false;
	}
}

class MyFileSystemPanel extends JPanel implements KeyListener, ActionListener {
	public JTextField typingField = new JTextField(60);
	public JTextArea displayArea = new JTextArea("HeroFay:>", 25, 60);
	// 60 columns, 25 rows
	String commandList[] =
		new String[] {
			"MKDIR",
			"Creates a directory.",
			"RMDIR",
			"Removes a directory.",
			"DIR",
			"Displays a list of files and subdirectories in a directory.",
			"CD",
			"Displays the name of or changes the current directory.",
			"NEW",
			"Create a file.",
			"OPEN",
			"Open a file.",
			"CLOSE",
			"Close a file.",
			"DEL",
			"Deletes one or more files.",
			"FORMAT",
			"Format the file system.",
			"DISKINFO",
			"Get Disk Information.",
			};

	public ArrayList currentPath = new ArrayList();
	public ArrayList path = new ArrayList();

	public SystemManager m = new SystemManager();

	public FileEditorPanel tp = new FileEditorPanel();
	public String command;

	MyFileSystemPanel() {
		currentPath.add("HeroFay:");
		path.add(new Character((char) - 1));

		Box box = Box.createVerticalBox();

		displayArea.setText(this.getCurrentPath());
		displayArea.setLineWrap(true);
		displayArea.setFont(new Font("黑体", Font.BOLD, 15));
		displayArea.setEditable(false);
		JScrollPane displayAreaScrollPanel = new JScrollPane(displayArea);
		JLabel label1 = new JLabel("Output:");
		Box box1 = Box.createHorizontalBox();
		box1.add(label1);
		box1.add(Box.createHorizontalGlue());
		box.add(box1);
		box.add(displayAreaScrollPanel);

		JButton clearButton = new JButton("Clear");
		Box box3 = Box.createHorizontalBox();
		clearButton.addActionListener(this);
		box3.add(clearButton);
		box.add(box3);

		box.add(Box.createVerticalStrut(5));
		JLabel label2 = new JLabel("Input:");
		Box box2 = Box.createHorizontalBox();
		box2.add(label2);
		box2.add(Box.createHorizontalGlue());
		box.add(box2);
		box.add(typingField);

		typingField.addKeyListener(this);
		this.add(box);
	}

	public void printHelp() {
		int i, maxLength = 0;
		for (i = 0; i < this.commandList.length; i += 2) {
			int tmp;
			if ((tmp = this.commandList[i].length()) > maxLength)
				maxLength = tmp;
		}
		//System.out.println(""+maxLength);
		for (i = 0; i < this.commandList.length; i += 2) {
			String str = this.commandList[i];
			int j;
			for (j = str.length(); j < maxLength + 1; j++) {
				str += " ";
			}
			System.out.println(str + this.commandList[i + 1] + "\n");
			displayArea.append(str + this.commandList[i + 1] + "\n");
		}
	}

	public void printError(String c) {
		displayArea.append("'" + c + "' 不是内部或外部命令，也不是可运行的程序\n");
		displayArea.append("或批处理文件。\n");
	}

	public void printDiskInfo() {
		displayArea.append(m.getDiskInfo());
	}

	public String getCurrentPath() {
		String pathStr = "";
		int i;
		currentPath.clear();
		currentPath.add(new String("HeroFay:"));
		char[][] temp = new char[SystemManager.cSector / 16][16];
		int j;
		for (i = 0; i < 32; i++) {
			for (j = 0; j < 16; j++) {
				temp[i][j] = m.rootDirectory[i][j];
			}
		}
		//根据路径得出目录层次的字符串。
		System.out.print("getCurrentPath 1:");
		for (i = 0; i < path.size(); i++) {
			System.out.print(
				"," + (int) ((Character) (path.get(i))).charValue());
		}
		System.out.println("");
		m.printSectorInfo(temp);
		System.out.println("" + path.size());

		for (i = 1; i < path.size(); i++) {
			if (i == 1) {
				System.out.println("i==1");
				char index = ((Character) (path.get(i))).charValue();
				for (j = 0; j < SystemManager.cSector / 16; j++) {
					System.out.println("" + j + ",index:" + (int) index);
					if (temp[j][SystemManager.stateIndex]
						!= SystemManager.isUsable_RCD
						&& m.getFatIndex_RCD(temp[j]) == index)
						break;
				}
				System.out.println("" + j);
				System.out.println(
					"m.printSectorInfo(temp); ,"
						+ (int) index
						+ ","
						+ (int) m.getFatIndex_RCD(temp[j]));
				m.printSectorInfo(temp);
				System.out.println("j:" + j + "," + m.getName(temp[j]));
				if (j < SystemManager.cSector / 16) {
					currentPath.add(m.getName(temp[j]));
					continue;
				}
			} else { // >= 2
				char indexDataArea =
					((Character) (path.get(i - 1))).charValue();
				int k;
				for (k = 0; k < SystemManager.cSector / 16; k++) {
					for (j = 0; j < 16; j++) {
						temp[k][j] = m.dataArea[indexDataArea][k * 16 + j];
					}
				}

				char index = ((Character) (path.get(i))).charValue();

				for (j = 0; j < SystemManager.cSector / 16; j++) {
					System.out.println("index:" + (int) index);
					System.out.println(
						"m.getFatIndex_RCD(temp[j])"
							+ (int) m.getFatIndex_RCD(temp[j]));
					if (temp[j][SystemManager.stateIndex]
						!= SystemManager.isUsable_RCD
						&& m.getFatIndex_RCD(temp[j]) == index)
						break;
				}
				if (j < SystemManager.cSector / 16) {
					System.out.println(
						"m.getName(temp[j])" + m.getName(temp[j]));
					System.out.println(
						"m.getName(temp[j][0])" + (int) temp[j][0]);
					System.out.println(
						"m.getName(temp[j][1])" + (int) temp[j][1]);
					System.out.println(
						"m.getName(temp[j][15])" + (int) temp[j][15]);
					currentPath.add(m.getName(temp[j]));
					continue;
				}

			}
		}

		for (i = 0; i < currentPath.size(); i++) {
			//	System.out.println(((String)currentPath.get(i)));
			if (i == 0)
				//pathStr += ((String) currentPath.get(i));
				pathStr += "HeroFay:";
			else
				pathStr += "\\" + ((String) currentPath.get(i));
		}

		pathStr += ">";
		return pathStr;
	}

	public void keyTyped(KeyEvent e) {
		if (e.getKeyChar() == KeyEvent.VK_ENTER) {
			//displayArea.append("Enter!!!\n");

			displayArea.append((command = typingField.getText()) + "\n");
			typingField.setText("");

			command = command.trim();
			if (command.toUpperCase().compareTo("HELP") == 0) {
				System.out.println(command);
				printHelp();
			} else if (
				command.length() >= 7
					&& command.substring(0, 5).toUpperCase().compareTo("MKDIR")
						== 0
					&& command.substring(5, 6).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];
				char indexDataArea = (char) - 1;

				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter = command.substring(6, command.length());
				boolean isSuccessful;
				if (indexDataArea != (char) - 1) {
					System.out.println("indexDataArea != (char) -1");
					isSuccessful = m.mkdir(indexDataArea, temp, parameter);
				} else {
					System.out.println("indexDataArea == (char) -1");
					isSuccessful = m.mkdir(temp, parameter);
				}
				if (!isSuccessful)
					displayArea.append("Failed\n");
			} else if (
				command.length() >= 7
					&& command.substring(0, 5).toUpperCase().compareTo("RMDIR")
						== 0
					&& command.substring(5, 6).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];
				char indexDataArea = (char) - 1;

				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter = command.substring(6, command.length());
				boolean isSuccessful;
				if (indexDataArea != (char) - 1) {
					System.out.println("indexDataArea != (char) -1");
					isSuccessful = m.rmdir(indexDataArea, temp, parameter);
				} else {
					System.out.println("indexDataArea == (char) -1");
					isSuccessful = m.rmdir(temp, parameter);
				}
				if (!isSuccessful)
					displayArea.append("Failed\n");
			} else if (command.toUpperCase().compareTo("DIR") == 0) {
				System.out.println(command);

				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					displayArea.append(m.dir(m.rootDirectory) + "\n");
				else {
					System.out.println("dd: " + path.size());

					char indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					System.out.println("ddd: " + path.size());

					char[][] temp = new char[SystemManager.cSector / 16][16];
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							//System.out.println("(i,j),i*16+j:
							// "+i+","+j+","+i*16+j);
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
							System.out.print("" + (int) temp[i][j] + ",");
						}
						System.out.println("");
					}
					System.out.println("dddd: " + path.size());
					displayArea.append(m.dir(temp) + "\n");

					System.out.println("ddddd: " + path.size());
				}
			} else if (
				command.length() >= 4
					&& command.substring(0, 2).toUpperCase().compareTo("CD") == 0
					&& command.substring(2, 3).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];

				char indexDataArea = (char) - 1;
				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter = command.substring(3, command.length());
				//boolean isSuccessful = m.mkdir(temp, parameter);
				//boolean isSuccessful = m.newFile(temp, parameter);
				boolean isSuccessful;
				if (indexDataArea == (char) - 1)
					isSuccessful = m.cdToFolder(temp, parameter);
				else
					isSuccessful = m.cdToFolder(indexDataArea, temp, parameter);
				if (!isSuccessful)
					displayArea.append("Failed\n");
			} else if (
				command.length() >= 5
					&& command.substring(0, 3).toUpperCase().compareTo("NEW") == 0
					&& command.substring(3, 4).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];
				System.out.println("pp: " + (int) path.size());

				char indexDataArea = (char) - 1;
				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					System.out.println("vv: " + (int) path.size());

					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				System.out.println("xx: " + (int) path.size());
				String parameter = command.substring(4, command.length());
				//boolean isSuccessful = m.mkdir(temp, parameter);
				boolean isSuccessful;
				if (indexDataArea == (char) - 1)
					isSuccessful = m.newFile(temp, parameter);
				else
					isSuccessful = m.newFile(indexDataArea, temp, parameter);
				if (!isSuccessful)
					displayArea.append("Failed\n");

			} else if (
				command.length() >= 6
					&& command.substring(0, 4).toUpperCase().compareTo("OPEN")
						== 0
					&& command.substring(4, 5).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];

				char indexDataArea = (char) - 1;
				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter = command.substring(5, command.length());
				boolean isSuccessful;
				String text = "";

				StringBuffer strBuffer;
				if (indexDataArea == (char) - 1) {
					//System.out.println("Before Reading File:");
					m.printSectorInfo(m.rootDirectory);
					strBuffer = new StringBuffer();
					isSuccessful = m.readFileToText(temp, parameter, strBuffer);
				} else {
					//System.out.println("Before Reading File:");
					m.printSectorInfo(m.dataArea[indexDataArea]);
					strBuffer = new StringBuffer();
					isSuccessful =
						m.readFileToText(
							indexDataArea,
							temp,
							parameter,
							strBuffer);
				}

				if (!isSuccessful)
					displayArea.append("Open Failed\n");
				else {
					text = strBuffer.toString();
					//	displayArea.append("File:\n"+text+"\n");

					boolean flag;
					if (indexDataArea == (char) - 1) {
						flag = m.del(temp, parameter);
						int i, j;
						for (i = 0; i < SystemManager.cSector / 16; i++) {
							for (j = 0; j < 16; j++) {
								temp[i][j] = m.rootDirectory[i][j];
							}
						}
						flag = m.newFile(temp, parameter) && flag;
					} else {
						flag = m.del(indexDataArea, temp, parameter);
						int i, j;
						for (i = 0; i < SystemManager.cSector / 16; i++) {
							for (j = 0; j < 16; j++) {
								temp[i][j] =
									m.dataArea[indexDataArea][i * 16 + j];
							}
						}
						flag =
							m.newFile(indexDataArea, temp, parameter) && flag;
					}

					System.out.println("Flag is " + flag);

					tp.oldText = tp.newText = text;
					tp.displayArea.setText(text);
					tp.f.tabs.setEnabledAt(0, false);
					tp.f.tabs.setEnabledAt(1, true);
					tp.f.tabs.setSelectedIndex(1);
					tp.lb.setText("File : " + parameter);
					tp.displayArea.requestFocusInWindow();
				}
			} else if (command.toUpperCase().compareTo("CLOSE") == 0) {
				System.out.println(command);

			} else if (
				command.length() >= 5
					&& command.substring(0, 3).toUpperCase().compareTo("DEL") == 0
					&& command.substring(3, 4).toUpperCase().compareTo(" ")
						== 0) {
				System.out.println(command);

				char[][] temp = new char[SystemManager.cSector / 16][16];

				char indexDataArea = (char) - 1;
				if (path.size() == 1
					&& ((Character) (path.get(path.size() - 1))).charValue()
						== (char) - 1)
					temp = m.rootDirectory;
				else {
					indexDataArea =
						((Character) (path.get(path.size() - 1))).charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] = m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter = command.substring(4, command.length());
				//boolean isSuccessful = m.mkdir(temp, parameter);
				//boolean isSuccessful = m.newFile(temp, parameter);
				boolean isSuccessful;
				if (indexDataArea == (char) - 1)
					isSuccessful = m.del(temp, parameter);
				else
					isSuccessful = m.del(indexDataArea, temp, parameter);
				if (!isSuccessful)
					displayArea.append("Failed\n");
			} else if (command.toUpperCase().compareTo("FORMAT") == 0) {
				System.out.println(command);
				m.format();
				while (path.size() > 1)
					path.remove(path.size() - 1);
				while (currentPath.size() > 1)
					currentPath.remove(currentPath.size() - 1);
			} else if (command.toUpperCase().compareTo("DISKINFO") == 0) {
				printDiskInfo();
			} else {
				printError(command);
			}

			displayArea.append("\n" + getCurrentPath());
			displayArea.setCaretPosition(displayArea.getDocument().getLength());
		}
	}

	public void keyPressed(KeyEvent e) {
	}

	public void keyReleased(KeyEvent e) {
	}

	public void actionPerformed(ActionEvent e) {
		displayArea.setText(this.getCurrentPath());
		typingField.requestFocusInWindow();
	}
}

class FileEditorPanel extends JPanel {
	public JTextArea displayArea = new JTextArea("", 28, 70);
	public JLabel lb = new JLabel();
	String filename = "File";
	public MyFileSystemFrame f = null;
	public String oldText = "";
	public String newText = "";

	public FileEditorPanel() {
		Box box = Box.createVerticalBox();

		lb.setText(filename + " :");
		Box box0 = Box.createHorizontalBox();
		box0.add(lb);
		box0.add(Box.createHorizontalGlue());

		box.add(box0);

		Box box1 = Box.createHorizontalBox();
		box1.add(displayArea);
		displayArea.setLineWrap(true);
		displayArea.setFont(new Font("黑体", Font.BOLD, 15));
		displayArea.setEditable(true);
		JScrollPane displayAreaScrollPanel = new JScrollPane(displayArea);
		box1.add(displayAreaScrollPanel);
		box.add(box1);

		Box box2 = Box.createHorizontalBox();
		JButton clear = new JButton("Clear");
		JButton save = new JButton("Save");
		JButton close = new JButton("Close");

		save.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				// 储存文件。
				//newText = f.p.tp.displayArea.getText();
				int fileSize = newText.length();

				char[][] temp = new char[SystemManager.cSector / 16][16];

				char indexDataArea = (char) - 1;
				if (f.p.path.size() == 1
					&& ((Character) (f.p.path.get(f.p.path.size() - 1)))
						.charValue()
						== (char) - 1)
					temp = f.p.m.rootDirectory;
				else {
					indexDataArea =
						((Character) (f.p.path.get(f.p.path.size() - 1)))
							.charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] =
								f.p.m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter =
					f.p.command.substring(5, f.p.command.length());
				boolean isSuccessful;
				String text = newText = displayArea.getText();
				if (indexDataArea == (char) - 1)
					isSuccessful = f.p.m.writeTextToFile(temp, parameter, text);
				else
					isSuccessful =
						f.p.m.writeTextToFile(
							indexDataArea,
							temp,
							parameter,
							text);

				if (!isSuccessful) {
					f.p.displayArea.append("\nSave Failed\n");
				} else {
					oldText = text;
				}
			}
		});
		box2.add(save);

		box2.add(Box.createHorizontalGlue());
		clear.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				displayArea.setText("");
			}
		});
		box2.add(clear);
		box2.add(Box.createHorizontalGlue());

		close.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				// 储存文件。
				//newText = f.p.tp.displayArea.getText();

				int fileSize = newText.length();

				char[][] temp = new char[SystemManager.cSector / 16][16];

				char indexDataArea = (char) - 1;
				if (f.p.path.size() == 1
					&& ((Character) (f.p.path.get(f.p.path.size() - 1)))
						.charValue()
						== (char) - 1)
					temp = f.p.m.rootDirectory;
				else {
					indexDataArea =
						((Character) (f.p.path.get(f.p.path.size() - 1)))
							.charValue();
					int i, j;
					for (i = 0; i < SystemManager.cSector / 16; i++) {
						for (j = 0; j < 16; j++) {
							temp[i][j] =
								f.p.m.dataArea[indexDataArea][i * 16 + j];
						}
					}
				}

				String parameter =
					f.p.command.substring(5, f.p.command.length());
				boolean isSuccessful;

				String text = oldText; //**

				if (indexDataArea == (char) - 1)
					isSuccessful = f.p.m.writeTextToFile(temp, parameter, text);
				else
					isSuccessful =
						f.p.m.writeTextToFile(
							indexDataArea,
							temp,
							parameter,
							text);

				if (!isSuccessful) {
					f.p.displayArea.append("\nClose Save Failed\n");
				} else {
					oldText = text;
				}

				f.tabs.setEnabledAt(1, false);
				f.tabs.setEnabledAt(0, true);
				f.tabs.setSelectedIndex(0);

				f.p.typingField.requestFocus();
				f.p.typingField.requestFocusInWindow();
			}
		});
		box2.add(close);
		box.add(box2);

		add(box);
	}
}

class MyFileSystemFrame extends JFrame {
	public MyFileSystemFrame f = null;
	public MyFileSystemPanel p = null;
	public JTabbedPane tabs = null;

	public MyFileSystemFrame() {
		f = this;
		setTitle("MyFileSystem");
		p = new MyFileSystemPanel();
		tabs = new JTabbedPane();
		tabs.addTab("FileSystem", p);
		tabs.addTab("TextEditor", p.tp);
		f.getContentPane().add(tabs);
		p.m.p = p; //让它持有自己的句柄。
		p.tp.f = this;

		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent arg0) {
				if (f.tabs.getSelectedIndex() == 1) {
					System.out.println("close");
					int fileSize = p.tp.newText.length();

					char[][] temp = new char[SystemManager.cSector / 16][16];

					char indexDataArea = (char) - 1;
					if (f.p.path.size() == 1
						&& ((Character) (f.p.path.get(f.p.path.size() - 1)))
							.charValue()
							== (char) - 1)
						temp = f.p.m.rootDirectory;
					else {
						indexDataArea =
							((Character) (f.p.path.get(f.p.path.size() - 1)))
								.charValue();
						int i, j;
						for (i = 0; i < SystemManager.cSector / 16; i++) {
							for (j = 0; j < 16; j++) {
								temp[i][j] =
									f.p.m.dataArea[indexDataArea][i * 16 + j];
							}
						}
					}

					String parameter =
						f.p.command.substring(5, f.p.command.length());
					boolean isSuccessful;

					String text = p.tp.oldText; //**

					if (indexDataArea == (char) - 1)
						isSuccessful =
							f.p.m.writeTextToFile(temp, parameter, text);
					else
						isSuccessful =
							f.p.m.writeTextToFile(
								indexDataArea,
								temp,
								parameter,
								text);

					if (!isSuccessful) {
						f.p.displayArea.append("\nClose Save Failed\n");
					} else {
						p.tp.oldText = text;
					}
				}
				//
				try {
					DataOutputStream out =
						new DataOutputStream(
							new BufferedOutputStream(
								new FileOutputStream("data")));

					for (int i = 0; i < p.m.rootDirectory.length; i++)
						for (int j = 0; j < 16; j++)
							out.writeChar(p.m.rootDirectory[i][j]);
					//p.m.printSectorInfo(p.m.rootDirectory);
					for (int i = 0; i < p.m.fatTable.length; i++) {
						out.writeChar(p.m.fatTable[i]);
						//	System.out.println(p.m.fatTable[i]+"");
					}

					for (int j = 0; j < p.m.dataArea.length; j++) {
						for (int i = 0; i < 512; i++)
							out.writeChar(p.m.dataArea[j][i]);
						//	p.m.printSectorInfo(p.m.dataArea[j]);
					}

					out.close();
				} catch (Exception e) {
					System.out.println(e);
				}
			}

			public void windowOpened(WindowEvent arg0) {
				System.out.println("open");
				try {
					DataInputStream in =
						new DataInputStream(
							new BufferedInputStream(
								new FileInputStream("data")));

					for (int i = 0; i < p.m.rootDirectory.length; i++)
						for (int j = 0; j < 16; j++)
							p.m.rootDirectory[i][j] = in.readChar();

					//	p.m.printSectorInfo(p.m.rootDirectory);
					for (int i = 0; i < p.m.fatTable.length; i++) {
						p.m.fatTable[i] = in.readChar();
						//	System.out.println(p.m.fatTable[i]+"");
					}

					for (int j = 0; j < p.m.dataArea.length; j++) {
						for (int i = 0; i < 512; i++)
							p.m.dataArea[j][i] = in.readChar();

						//	p.m.printSectorInfo(p.m.dataArea[j]);
					}
					in.close();

				} catch (Exception e) {
					System.out.println(e);
				}
			}
		});
	}
}

public class MyFileSystem {
	public static void main(String[] args) {
		JFrame.setDefaultLookAndFeelDecorated(true);

		MyFileSystemFrame f = new MyFileSystemFrame();
		Console.run(f);
		f.setResizable(false);
		f.p.typingField.requestFocusInWindow();
		f.tabs.setEnabledAt(1, false);
		//f.tabs.setSelectedIndex(1);
		//f.p.tp.displayArea.requestFocusInWindow();
	}
}
