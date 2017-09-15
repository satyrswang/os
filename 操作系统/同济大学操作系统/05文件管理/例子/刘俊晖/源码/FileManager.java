import java.util.*;
//import javax.swing.*;

/*
 * Created on 2004-6-29
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author ������
 *
 * �ļ�ϵͳ���ں�
 */
public class FileManager {

	public static void main(String[] args) {
		//-----for test-----//

		//FileManager fileManager = new FileManager();
		//fileManager.testCreate();
		//fileManager.testChangeDir();
		//fileManager.testModify();
		//fileManager.testDelete();
	}

	//////////////////////////////////////
	//��Ŀ¼����ά�ռ䣨Ĭ�ϴ�Сռ1�������� 
	char[][] rootTable;

	//fat��,��������������   
	char[] fatTable;

	//�����������ڴ���ı��ļ�������Ŀ¼��Ϣ
	char[][] dataArea;

	//��¼��ǰĿ¼��Ϣ��ջ
	ArrayList directoryStack;

	//��¼��ǰ��·��
	ArrayList path;

	//////////////////////////////////////
	private void test() {
	}

	//���캯������ʼ��һЩ��Ϣ
	public FileManager() {
		System.out.println("Create filemanager!");
		rootTable = new char[Utility.NUM_OF_ROOTFILE][Utility.SIZE_OF_FILEINFO];
		fatTable = new char[Utility.NUM_OF_DATASECTOR];
		dataArea = new char[Utility.NUM_OF_DATASECTOR][Utility.SIZE_OF_SECTOR];

		this.formatAll();

	}

	/**
	 * �����ļ��洢����
	 */
	private boolean deploy() {

		return true;
	}

	/**
	 * ��ʽ���������Ϣ
	 */
	//��ʽ��������Ϣ����Ҫ��Ը�Ŀ¼���Fat��
	void formatAll() {
		//���ļ���Ϣ��״̬���ʾΪ���ã�������Ϣ���ٴ�ʹ�õ�ʱ�������
		for (int i = 0; i < Utility.NUM_OF_ROOTFILE; i++) {
			rootTable[i][Utility.POS_STATE] = Utility.FREE;
			rootTable[i][Utility.POS_FAT] = Utility.FREE_FOR_FAT;
		}

		//���Fat��û�б�Ҫ��ʱ��������������ݣ�Fat�����Ϣ�ɿ�������������Ϣ��
		for (int i = 0; i < Utility.NUM_OF_DATASECTOR; i++) {
			fatTable[i] = Utility.FREE_FOR_FAT;
		}

		//�½�Ŀ¼ջ
		directoryStack = new ArrayList();
		directoryStack.add(new Integer(-1)); //��ʼΪ��Ŀ¼
		path = new ArrayList();
		path.add("MyRoot:");
		//System.out.println("stack = " + directoryStack.size());
	}

	//��ʽ����Ӧ������������������(���ڴ������)
	void formatSectorForData(int freeIndex) {
		for (int i = 0; i < Utility.SIZE_OF_SECTOR; i++) {
			dataArea[freeIndex][i] = 0;
		}
	}

	//��ʽ����Ӧ������������������(���ڴ��Ŀ¼��Ϣ)
	void formatSectorForDir(int freeIndex) {
		char[] sector = dataArea[freeIndex];
		for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
			sector[i * Utility.SIZE_OF_FILEINFO + Utility.POS_STATE] =
				Utility.FREE;
			sector[i * Utility.SIZE_OF_FILEINFO + Utility.POS_FAT] =
				Utility.FREE_FOR_FAT;
		}
	}

	/**
	 * �½�Ŀ¼���ļ���Ϣ��
	 */
	boolean createInfo(char type, String name) {
		if (name.length() > 12) {
			return false;
		}
		int currentDirectory =
			((Integer) directoryStack.get(directoryStack.size() - 1))
				.intValue();
		int fatIndex;

		if (this.inDirectory(type, name, false) != -1) {
			System.out.println(
				"collide fatIndex:" + inDirectory(type, name, false));
			return false;
		}

		//�ڸ�Ŀ¼��
		if (currentDirectory == -1) {
			for (int i = 0; i < Utility.NUM_OF_ROOTFILE; i++) {
				if (rootTable[i][Utility.POS_STATE] == Utility.FREE) {
					if ((fatIndex = getFreeSector(type)) != -1) {
						rootTable[i][Utility.POS_FAT] = (char) fatIndex;
						//format
						rootTable[i][Utility.POS_STATE] = type;
						this.changeNameOfFileInfo(
							rootTable[i],
							Utility.POS_NAME,
							name);
						return true;
					}
					return false;
				}
			}
			return false;
		}
		//����Ŀ¼��
		char[] subDirectory = dataArea[currentDirectory];

		for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
			if (subDirectory[i * Utility.SIZE_OF_FILEINFO + Utility.POS_STATE]
				== Utility.FREE) {
				if ((fatIndex = getFreeSector(type)) != -1) {
					subDirectory[i * Utility.SIZE_OF_FILEINFO
						+ Utility.POS_FAT] =
						(char) fatIndex;
					subDirectory[i * Utility.SIZE_OF_FILEINFO
						+ Utility.POS_STATE] =
						type;
					this.changeNameOfFileInfo(
						subDirectory,
						i * Utility.SIZE_OF_FILEINFO,
						name);
					return true;
				}
				return false;
			}
		}
		return false;
	}

	private void testCreate() {
		System.out.println("------testCreate--------");
		this.createInfo(Utility.FILE, "file1");
		//this.createInfo(Utility.FILE, "file2");
		//this.createInfo(Utility.FILE, "file3");
		//this.showCurrentDirInfo();
		this.createInfo(Utility.DIRECTORY, "dir1");
		//this.createInfo(Utility.DIRECTORY, "dir2");
		//this.showCurrentDirInfo();
	}

	//��Fat���ҿ�������,�Ҳ�������-1
	int getFreeSector(char type) {
		for (int i = 0; i < Utility.NUM_OF_DATASECTOR; i++) {
			if (fatTable[i] == Utility.FREE_FOR_FAT) {
				if (type == Utility.FILE) {
					this.formatSectorForData(i);
				} else if (type == Utility.DIRECTORY) {
					this.formatSectorForDir(i);
				}
				fatTable[i] = Utility.END_OF_FILE;
				return i;
			}
		}
		return -1;
	}

	//�޸ĸ�Ŀ¼�ļ���Ϣ�е�������Ϣ
	void changeNameOfFileInfo(char[] fileInfo, int index, String name) {
		for (int i = 0; i < Utility.LEN_OF_NAME; i++) {
			fileInfo[index + i] = 0;
		}
		for (int i = 0; i < name.length(); i++) {
			fileInfo[index + i] = name.charAt(i);
		}
	}

	/**
	 * �޸��ļ����������
	 */
	public boolean loadFile(String name, StringBuffer content) {
		int fatIndex = inDirectory(Utility.FILE, name, false);
		if (fatIndex == -1) {
			return false;
		}

		int nextIndex = fatTable[fatIndex];
		while (true) {
			content.append(
				String
					.valueOf(dataArea[fatIndex], 0, Utility.SIZE_OF_SECTOR)
					.trim());
			System.out.println("file content : " + content.toString());
			if (nextIndex == Utility.END_OF_FILE) {
				return true;
			}
			fatIndex = nextIndex;
			nextIndex = fatTable[fatIndex];
		}
	}

	public boolean writeFile(String name, String content) {
		int fatIndex = inDirectory(Utility.FILE, name, false);
		if (fatIndex == -1) {
			return false;
		}
		if (content.length() == 0) {
			return true;
		}

		int objectTotal =
			content.length() % Utility.SIZE_OF_SECTOR == 0
				? content.length() / Utility.SIZE_OF_SECTOR
				: content.length() / Utility.SIZE_OF_SECTOR + 1;

		if (this.modifySector(name, objectTotal) == false) {
			return false;
		}

		int bufferIndex = 0;
		int bufferLeft = content.length();

		for (int i = 0; i < objectTotal; i++) {
			if (i == objectTotal - 1) {
				for (int j = 0; j < bufferLeft; j++) {
					char a = content.charAt(bufferIndex++);
					this.dataArea[fatIndex][j] = a;
				}
			} else {
				for (int k = 0; k < Utility.SIZE_OF_SECTOR; k++) {
					this.dataArea[fatIndex][k] = content.charAt(bufferIndex++);
				}
			}
			bufferLeft = bufferLeft - Utility.SIZE_OF_SECTOR;
			fatIndex = fatTable[fatIndex];
		}
		return true;
	}

	//����Ŀ���С�޸��ļ�������
	boolean modifySector(String name, int objectTotal) {
		int fatIndex = this.inDirectory(Utility.FILE, name, false);
		if (fatIndex == -1) {
			return false;
		}
		showFatList(fatIndex);

		int initIndex = fatIndex;
		int[] fatArray = new int[Utility.MAX_SECTOR];
		int arrayIndex = -1;

		fatArray[++arrayIndex] = fatIndex;
		while (true) {
			if (fatTable[fatIndex] != Utility.END_OF_FILE) {
				fatArray[++arrayIndex] = fatTable[fatIndex];
				fatIndex = fatArray[arrayIndex];
			} else
				break;
		}
		int orientTotal = arrayIndex + 1;
		/*System.out.println(
			"orient total : "
				+ orientTotal
				+ "   object total : "
				+ objectTotal);*/
		int diff = Math.abs(objectTotal - orientTotal);
		if (objectTotal > orientTotal) {
			int freeIndex;
			for (int i = 0; i < diff; i++) {
				freeIndex = getFreeSector(Utility.FILE);
				if (freeIndex == -1) {
					return false;
				}
				fatArray[++arrayIndex] = freeIndex;
			}
			//��֤�˿ռ��㹻
			arrayIndex = orientTotal - 1;
			for (int i = 0; i < diff; i++) {
				fatTable[fatArray[arrayIndex]] =
					(char) fatArray[arrayIndex + 1];
				arrayIndex++;
			}
			fatTable[fatArray[arrayIndex]] = Utility.END_OF_FILE;
			showFatList(initIndex);
		}
		if (objectTotal < orientTotal) {
			arrayIndex = objectTotal - 1;
			fatTable[arrayIndex] = Utility.END_OF_FILE;
			for (int i = 1; i <= diff; i++) {
				fatTable[fatArray[arrayIndex + i]] = Utility.FREE_FOR_FAT;
			}
			showFatList(initIndex);
		}
		return true;
	}

	private void showFatList(int fatIndex) {
		System.out.print("Fat List : " + fatIndex);
		while (fatTable[fatIndex] != Utility.END_OF_FILE) {
			fatIndex = fatTable[fatIndex];
			System.out.print(" " + fatIndex);
		}
		System.out.println(" " + fatTable[fatIndex]);
	}

	private void testModify() {
		System.out.println("------testModify--------");
		this.showCurrentDirInfo();
		this.createInfo(Utility.FILE, "file1");
		this.createInfo(Utility.DIRECTORY, "dir1");
		this.showCurrentDirInfo();
		this.changeDirectory("dir1");
		this.createInfo(Utility.FILE, "dir1");
		this.showCurrentDirInfo();
		modifySector("dir1", 3);
		this.showCurrentDirInfo();
	}

	/**
	 * ɾ��Ŀ¼���ļ���Ϣ��
	 */
	//ɾ����ǰĿ¼�µ��ļ�����Ŀ¼
	boolean deleteInfo(char type, String name) {
		if (type == Utility.FILE) {
			int fatIndex = this.inDirectory(Utility.FILE, name, true);
			if (fatIndex != -1) {
				deleteSectorList(fatIndex);
				return true;
			}
		} else if (type == Utility.DIRECTORY) {
			int fatIndex = this.inDirectory(Utility.DIRECTORY, name, true);
			if (fatIndex != -1) {
				deleteSubDir(fatIndex);
				return true;
			}
		}
		return false;
	}

	private void deleteSubDir(int fatIndex) {
		deleteSectorList(fatIndex);
		char[] subDirectory = dataArea[fatIndex];
		for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
			int initPos = i * Utility.SIZE_OF_FILEINFO;
			if (subDirectory[initPos + Utility.POS_STATE] != Utility.FREE) {
				if (subDirectory[initPos + Utility.POS_STATE]
					== Utility.FILE) {
					deleteSectorList(subDirectory[initPos + Utility.POS_FAT]);
				} else if (
					subDirectory[initPos + Utility.POS_STATE]
						== Utility.DIRECTORY) {
					deleteSubDir(subDirectory[initPos + Utility.POS_FAT]);
				}
				subDirectory[initPos + Utility.POS_STATE] = Utility.FREE;
			}
		}
	}

	//��Fat�������ĳ���ļ���Ϣ��������
	void deleteSectorList(int firstIndex) {
		if (firstIndex < 0 || firstIndex > Utility.NUM_OF_DATASECTOR) {
			return;
		}

		int nextIndex = fatTable[firstIndex];
		while (true) {
			//	System.out.println(
			//		"allocFreeSector " + firstIndex + "  " + (int) nextIndex);
			fatTable[firstIndex] = Utility.FREE_FOR_FAT;
			if (nextIndex == Utility.END_OF_FILE) {
				return;
			}
			firstIndex = nextIndex;
			nextIndex = fatTable[firstIndex];
		}
	}

	private void testDelete() {
		System.out.println("------testDelete--------");
		this.showCurrentDirInfo();

		changeDirectory("dir1");
		this.showCurrentDirInfo();

		changeDirectory("dir2");
		this.showCurrentDirInfo();

		changeDirectory("dir3");
		this.showCurrentDirInfo();

		//////delete subdir
		changeDirectory("\\");
		this.showCurrentDirInfo();

		deleteInfo(Utility.FILE, "file1");
		this.showCurrentDirInfo();

		deleteInfo(Utility.DIRECTORY, "dir1");
		this.showCurrentDirInfo();

		//////recreate
		this.createInfo(Utility.FILE, "file6");
		this.createInfo(Utility.DIRECTORY, "dir6");
		this.showCurrentDirInfo();

		changeDirectory("dir6");
		this.createInfo(Utility.FILE, "file7");
		this.showCurrentDirInfo();
		//this.modifySector("file7",3);
		this.createInfo(Utility.DIRECTORY, "dir7");
		this.showCurrentDirInfo();

		this.changeDirectory("\\");
		this.deleteInfo(Utility.FILE, "file6");
		changeDirectory("dir6");
		changeDirectory("dir7");
		this.showCurrentDirInfo();

		this.createInfo(Utility.FILE, "file8");
		this.createInfo(Utility.DIRECTORY, "dir8");
		this.showCurrentDirInfo();

	}

	/**
	 * �ı䵱ǰĿ¼
	 */
	boolean changeDirectory(String name) {
		if (name.length() > 12) {
			return false;
		}

		//��Ŀ¼
		if (name.compareTo(".") == 0) {
			return true;
		}

		//������һ��Ŀ¼
		if (name.compareTo("..") == 0) {
			if (directoryStack.size() <= 1) {
				return true;
			}
			path.remove(path.size() - 1);
			directoryStack.remove(directoryStack.size() - 1);
			return true;
		}

		//���ظ�Ŀ¼
		if (name.compareTo("\\") == 0) {
			int deleteSize = directoryStack.size() - 1;
			for (int i = 1; i <= deleteSize; i++) {
				path.remove(1);
				directoryStack.remove(1);
			}
			return true;
		}

		//����һ���µ�Ŀ¼
		int fatIndex = inDirectory(Utility.DIRECTORY, name, false);
		if (fatIndex != -1) {
			path.add(name);
			directoryStack.add(new Integer(fatIndex));
			return true;
		}
		return false;
	}

	private void testChangeDir() {
		System.out.println("------testChangeDir--------");
		//this.showCurrentDirInfo();

		if (!changeDirectory("dir1"))
			System.out.println("can't change");
		//this.showCurrentDirInfo();

		this.createInfo(Utility.FILE, "file2");
		this.createInfo(Utility.DIRECTORY, "dir2");
		//this.showCurrentDirInfo();
		if (!changeDirectory("dir2"))
			System.out.println("can't change");
		//this.showCurrentDirInfo();

		this.createInfo(Utility.FILE, "file3");
		this.createInfo(Utility.DIRECTORY, "dir3");
		//this.showCurrentDirInfo();
		if (!changeDirectory("dir3"))
			System.out.println("can't change");
		//this.showCurrentDirInfo();

		if (!changeDirectory("\\"))
			System.out.println("can't change");
		//this.showCurrentDirInfo();
	}

	/**
	 * Ŀ¼��Ϣ��ȡ���ж�
	 */
	//�ж��ļ�����Ŀ¼�������Ƿ��ڵ�ǰĿ¼��
	int inDirectory(char type, String name, boolean delete) {
		String tempName;
		//��Ŀ¼��
		if (((Integer) directoryStack.get(directoryStack.size() - 1))
			.intValue()
			== -1) {
			if (type == Utility.FILE) {
				for (int i = 0; i < Utility.NUM_OF_ROOTFILE; i++) {
					if (rootTable[i][Utility.POS_STATE] == Utility.FILE) {
						tempName =
							new String(
								rootTable[i],
								Utility.POS_NAME,
								Utility.LEN_OF_NAME)
								.trim();
						if (tempName.compareTo(name) == 0) {
							if (delete == true) {
								rootTable[i][Utility.POS_STATE] = Utility.FREE;
							}
							return rootTable[i][Utility.POS_FAT];
						}
					}
				}
			} else if (type == Utility.DIRECTORY) {
				for (int i = 0; i < Utility.NUM_OF_ROOTFILE; i++) {
					if (rootTable[i][Utility.POS_STATE] == Utility.DIRECTORY) {
						tempName =
							new String(
								rootTable[i],
								Utility.POS_NAME,
								Utility.LEN_OF_NAME)
								.trim();
						if (tempName.compareTo(name) == 0) {
							if (delete == true) {
								rootTable[i][Utility.POS_STATE] = Utility.FREE;
							}
							return rootTable[i][Utility.POS_FAT];
						}
					}
				}
			}
			return -1;
		}

		//��ͨ��Ŀ¼��
		int fatIndex =
			((Integer) directoryStack.get(directoryStack.size() - 1))
				.intValue();
		char[] subDirectory = dataArea[fatIndex];

		if (type == Utility.FILE) {
			for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
				if (subDirectory[i * Utility.SIZE_OF_FILEINFO
					+ Utility.POS_STATE]
					== Utility.FILE) {
					tempName =
						new String(
							subDirectory,
							i * Utility.SIZE_OF_FILEINFO + Utility.POS_NAME,
							Utility.LEN_OF_NAME)
							.trim();
					if (tempName.compareTo(name) == 0) {
						if (delete == true) {
							subDirectory[i * Utility.SIZE_OF_FILEINFO
								+ Utility.POS_STATE] =
								Utility.FREE;
						}
						return subDirectory[i * Utility.SIZE_OF_FILEINFO
							+ Utility.POS_FAT];
					}
				}
			}
		} else if (type == Utility.DIRECTORY) {
			for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
				if (subDirectory[i * Utility.SIZE_OF_FILEINFO
					+ Utility.POS_STATE]
					== Utility.DIRECTORY) {
					tempName =
						new String(
							subDirectory,
							i * Utility.SIZE_OF_FILEINFO + Utility.POS_NAME,
							Utility.LEN_OF_NAME)
							.trim();
					if (tempName.compareTo(name) == 0) {
						if (delete == true) {
							subDirectory[i * Utility.SIZE_OF_FILEINFO
								+ Utility.POS_STATE] =
								Utility.FREE;
						}
						return subDirectory[i * Utility.SIZE_OF_FILEINFO
							+ Utility.POS_FAT];
					}
				}
			}
		}
		return -1;
	}

	//�õ���ǰĿ¼����Ϣ
	ArrayList getCurrentDirInfo() {
		ArrayList infoList = new ArrayList();
		int currentDirectory =
			((Integer) directoryStack.get(directoryStack.size() - 1))
				.intValue();
		System.out.println("currentDirectory : " + currentDirectory);
		if (currentDirectory == -1) {
			for (int i = 0; i < Utility.NUM_OF_ROOTFILE; i++) {
				if (rootTable[i][Utility.POS_STATE] != Utility.FREE) {
					infoList.add(new String(rootTable[i]));
				}
			}
			return infoList;
		}

		char[] subDirectory = dataArea[currentDirectory];
		for (int i = 0; i < Utility.NUM_OF_SUBFILE; i++) {
			if (subDirectory[i * Utility.SIZE_OF_FILEINFO + Utility.POS_STATE]
				!= Utility.FREE) {
				String addStr =
					new String(
						subDirectory,
						i * Utility.SIZE_OF_FILEINFO,
						Utility.SIZE_OF_FILEINFO);
				infoList.add(addStr);
			}
		}
		return infoList;
	}

	//��ʾ��ǰĿ¼��Ϣ
	private void showCurrentDirInfo() {
		ArrayList infoList = this.getCurrentDirInfo();
		String name;
		char state, fatIndex, size, readonly;
		for (int i = 0; i < infoList.size(); i++) {
			String temp = (String) infoList.get(i);
			name = temp.substring(0, 12);
			state = temp.charAt(12);
			fatIndex = temp.charAt(13);
			size = temp.charAt(14);
			readonly = temp.charAt(15);
			System.out.println(
				"Name : "
					+ name.trim()
					+ "  State:"
					+ (int) state
					+ "  Fat:"
					+ (int) fatIndex
					+ "  Size:"
					+ (int) size
					+ "  ReadOnly:"
					+ (int) readonly);
		}
		if (infoList.size() == 0) {
			System.out.println("Empty!!");
		}
		return;
	}

	public String getCurrentPath() {
		String pathStr = "", temp;
		for (int i = 0; i < path.size(); i++) {
			temp = (String) path.get(i);
			pathStr = pathStr + temp + "\\";
		}
		pathStr = pathStr + ">";
		System.out.println(pathStr);
		return pathStr;
	}
}
