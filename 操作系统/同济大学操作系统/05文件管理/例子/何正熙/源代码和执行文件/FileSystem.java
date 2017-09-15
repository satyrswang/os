/*
 * Created on 2004-4-29
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

import java.util.ArrayList;
import java.util.List;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class FileSystem {
	/*
	 * ����ṹ˵����
	 *   һ��������СΪ512 char
	 *   level 1����������Ϣ     ��С��1��������С
	 *   level 2����Ŀ¼�����   ��С��1��������С��32*16 ����32Ϊ��Ŀ¼һ����֧���ļ���Ŀ¼������16Ϊÿ��������Ϣ�ĳ��ȣ�
	 *   level 3��fat��          ��С��4��������С������2048���������ֱ��Ӧ��������2048������������
	 *   level 4��������         ��С��2048��������ÿ��������СΪ512 char
	 */

	final static int SECTOR = 512; // һ��������СΪ512 char 
	char[] diskInfo = new char[SECTOR]; // ��������Ϣ ��СΪһ������
	char[][] rootManageTable = new char[32][16]; // ��Ŀ¼����� ��СΪһ������
	char[] fat = new char[SECTOR * 4]; // fat���СΪ2048 char���ֱ��Ӧ��������2048������
	char[][] datafield = new char[2048][SECTOR]; // ������ 2048 * 512 

	final static int END = 0xffff; //����fat����, ��ʾ�������������ļ������һ������.
	final static int FREE = 0xfffe; //fat����ֵ����ʶ��fat���Ӧ����������Ϊ����
	int currentBlockIndex = 0;

	// ��ʽ��:��fat����� + ��Ŀ¼�������������Ϊ����
	public void format() {
		for (int i = 0; i < fat.length; i++) {
			fat[i] = FREE;
		}
		for (int i = 0; i < rootManageTable.length; i++) {
			rootManageTable[i][11] = 0;
		}
		currentBlockIndex = 0;
	}

	// �ڸ�Ŀ¼������Ŀ
	public boolean createRootItem(String name, int itemType) {
		name = name.trim();

		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the directory name has a limitation of 8 byte.");
			return false;
		}

		// �鿴��Ŀ¼�Ƿ���ͬ������Ŀ��
		for (int i = 0; i < rootManageTable.length; i++) {
			if (rootManageTable[i][11] == itemType) {
				if (String
					.valueOf(rootManageTable[i], 0, 8)
					.trim()
					.equals(name)) {
					System.out.println(
						"Error:There is a same named item in this current diretory.");
					return false;
				}
			}
		}

		// ���Ҹ�Ŀ¼�����Ŀ�����Ϣ�飬��¼��Ŀ��Ϣ
		for (int i = 0; i < rootManageTable.length; i++) {
			if (rootManageTable[i][11] == 0) {
				// д����Ŀ������
				for (int j = 0; j < name.length(); j++) {
					rootManageTable[i][j] = name.charAt(j);
				}
				for (int k = name.length(); k < 8; k++) {
					rootManageTable[i][k] = 0;
				}

				// �޸����ͱ�ǣ�Ŀ¼Ϊ��2�� �ļ�Ϊ��1
				rootManageTable[i][11] = (char) itemType;

				int temp = 0;
				if ((temp = allocFreeSector()) >= 0) {
					rootManageTable[i][12] = (char) temp;
				} else {
					System.out.println("Error: No free space");
				}

				System.out.println(
					" Create Root item success! The No. of sector is " + temp);
				return true;
			}
		}

		return false;
	}
	
	// �����ļ�
	public boolean saveFile(List path,String name,String content)
	{
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the file name has a limitation of 8 byte.");
			return false;
		}
		
		int fatFirstIndex = -1;
		// ������ļ��ڸ�Ŀ
		if (path.size()==0)
		{
			// ��fat����Ѱ��Ϊ���ļ������fatͷ��
			fatFirstIndex = getFirstFat(name, 1);
			if (fatFirstIndex == -1) {
				return false;
			}
		}
		else
		// ������ļ��ڷǸ�Ŀ¼
		if (path.size()>0)
		{
			fatFirstIndex = getInnerDirectorySector(path);
			if (fatFirstIndex == -1)
			{
				return false;
			}
			
			// Ŀ¼��������Ѱ��Ϊ���ļ������fatͷ��
			boolean tag = false;
			for (int i = 0; i < 32; i++) {
				if (datafield[fatFirstIndex][16 * i + 11] == 1) {
					if (String
						.valueOf(datafield[fatFirstIndex], 16 * i, 8)
						.trim()
						.equals(name)) {
						
							fatFirstIndex = datafield[fatFirstIndex][16*i + 12];
						tag = true;
						break;
					}
				}
			}
			if (!tag)
			{
				System.out.println("File " + name + " not found!");
			}
		}
		
		// Ϊfile�����㹻�Ŀռ�
		int fatIndex = fatFirstIndex;
		int blockNum = content.length() / 512; // Ӧ�÷��������������Ŀ
		if (content.length() % 512 != 0) // ���ϲ���һ�������Ĳ���
			{
			blockNum++;
		}

		for (int i = 1; i < blockNum; i++) // ��������
			{
			char temp = (char) allocFreeSector();
			fat[fatIndex] = temp;
			fatIndex = temp;
		}
	/*	
		void clearContent(int sect)
		{
			for (int i = 0; i < SECTOR; i++)
			{
				datafield[sect][i] = 0;
			}
		}*/

		// ������д���ļ�
		for (int i = 0; i < blockNum; i++) {
			clearContent(fatIndex);
			for (int j = 0; j < content.length(); j++) {
//				System.out.print(content.charAt(SECTOR * i + j));
				datafield[fatIndex][j] = content.charAt(SECTOR * i + j);
			}
			fatIndex = fat[fatIndex];
		}
//		System.out.println(fatIndex);
		
		return true;
	}
	
	// ��ȡ�ļ�
	public StringBuffer loadFile(List path,String name)
	{
		StringBuffer stringContainor = new StringBuffer();
		
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the file name has a limitation of 8 byte.");
			return null;
		}
		
		int fatFirstIndex = -1;
		// ��ȡ���ļ��ڸ�Ŀ
		if (path.size()==0)
		{
			// ��fat����Ѱ��Ϊ���ļ������fatͷ��
			fatFirstIndex = getFirstFat(name, 1);
			if (fatFirstIndex == -1) {
				return null;
			}
		}
		else
		// ��ȡ���ļ��ڷǸ�Ŀ¼
		if (path.size()>0)
		{
			fatFirstIndex = getInnerDirectorySector(path);
			if (fatFirstIndex == -1)
			{
				return null;
			}
			
			// Ŀ¼��������Ѱ��Ϊ���ļ������fatͷ��
			boolean tag = false;
			for (int i = 0; i < 32; i++) {
				if (datafield[fatFirstIndex][16 * i + 11] == 1) {
					if (String
						.valueOf(datafield[fatFirstIndex], 16 * i, 8)
						.trim()
						.equals(name)) {
						
							fatFirstIndex = datafield[fatFirstIndex][16*i + 12];
						tag = true;
						break;
					}
				}
			}
			if (!tag)
			{
				System.out.println("File " + name + " not found!");
				return null;
			}
		}
		
		//�����ݶ��뵽����
			 do {
				 stringContainor.append(
					 String.valueOf(datafield[fatFirstIndex], 0, SECTOR).trim());

				fatFirstIndex = fat[fatFirstIndex];
			 } while (fatFirstIndex != END);

//			 System.out.println(fatFirstIndex);
		
		return stringContainor;
	}

	//	 ɾ����Ŀ¼�ϵ���Ŀ
	public boolean deleteRootItem(String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}

		// �õ���Ŀ��ͷfat��
		int fatIndex = getFirstFat(name, itemType);
		if (fatIndex == -1) {
			return false;
		}

		// �޸�fat��ǣ���ʶΪ����
		int temp;
		do {
			temp = fat[fatIndex];
			fat[fatIndex] = FREE;
			fatIndex = temp;
		} while (fatIndex != END);

		// �޸ĸ�Ŀ¼���������Ϊ����
		for (int i = 0; i < rootManageTable.length; i++) {
			if (rootManageTable[i][11] == itemType)
				if (String
					.valueOf(rootManageTable[i], 0, 8)
					.trim()
					.equals(name)) {
					rootManageTable[i][11] = 0;
					rootManageTable[i][12] = 0;

					System.out.println(
						"item " + name + " has been successfully deleted!");
					break;
				}
		}
		return true;
	}


	// �ڷǸ�Ŀ¼������Ŀ
	public boolean createItem(List path, String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}

		// �ڸ�Ŀ¼����
		if (path.size() == 0) {

		}
		// �ڷǸ�Ŀ¼����
		else if (path.size() > 0) {
			
			// �ҵ���¼��ǰĿ¼����������index
			int fatIndex = getInnerDirectorySector(path);
			if (fatIndex == -1)
			{
				return false;
			}

			// ��鵱ǰĿ¼�Ƿ�����ͬ����Ŀ��
			for (int i = 0; i < 32; i++) {
				if (datafield[fatIndex][16 * i + 11] == itemType) {
					if (String
						.valueOf(datafield[fatIndex], 16 * i, 8)
						.trim()
						.equals(name)) {
						System.out.println(
							"Error:There is a same named item in this current diretory.");
						return false;
					}
				}
			}

			// �ڵ�ǰĿ¼������¼Ŀ¼��Ϣ
			for (int i = 0; i < 32; i++) {
				if (datafield[fatIndex][16 * i + 11] == 0) {
					// д��Ŀ¼����
					for (int j = 0; j < name.length(); j++) {
						datafield[fatIndex][16 * i + j] = name.charAt(j);
					}
					for (int k = name.length(); k < 8; k++) {
						datafield[fatIndex][16 * i + k] = 0;
					}

					// �޸����ͱ�ǣ�Ŀ¼Ϊ��2�� �ļ�Ϊ��1
					datafield[fatIndex][16 * i + 11] = (char) itemType;

					int temp = 0;
					if ((temp = allocFreeSector()) >= 0) {
						datafield[fatIndex][16 * i + 12] = (char) temp;
					} else {
						System.out.println("Error: No free space");
					}

					System.out.println(
						" Create item "
							+ name
							+ " success! The No. of sector is "
							+ temp);
					return true;
				}
			}
		}
		return true;
	}

	//	 ɾ���Ǹ�Ŀ¼�ϵ���Ŀ
	public boolean deleteItem(List path, String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}
		
		// �ҵ���¼��ǰĿ¼����������index
		int fatIndex = getInnerDirectorySector(path);
		if (fatIndex == -1)
		{
			return false;
		}

		// ��鵱ǰĿ¼�Ƿ��д���Ŀ,���ͷfat����
		int itemIndex = -1;
		int fatTemp = -1;
		for (int i = 0; i < 32; i++) {
			if (datafield[fatIndex][16 * i + 11] == itemType) {
				if (String
					.valueOf(datafield[fatIndex], 16 * i, 8)
					.trim()
					.equals(name)) {
						
					System.out.println(
						"Get the item!");
					
					itemIndex = i;  // ���µ�ǰ��Ŀ¼��¼��
					fatTemp = fatIndex;  // ���µ�ǰĿ¼��¼������
					
					fatIndex = (int)datafield[fatIndex][16*i + 12];
					
					break;
				}
			}
		}
		if (itemIndex == -1)
		{
			System.out.println("no such item!");
			return false;
		}

		// �޸�fat��ǣ���ʶΪ����
		int temp;
		do {
			temp = fat[fatIndex];
			fat[fatIndex] = FREE;
			fatIndex = temp;
		} while (fatIndex != END);

		// �޸ĸ�Ŀ¼���������Ϊ����
		datafield[fatTemp][16*itemIndex +11] = 0;
		datafield[fatTemp][16*itemIndex +12] = 0;

		System.out.println(
		"item " + name + " has been successfully deleted!");

		return true;
	}
	

	// �ڸ�Ŀ¼��������Ѱ��Ϊ�ļ���Ŀ¼�����fatͷ�� (tag ��ʶ 1 Ϊ�ļ���2 ΪĿ¼)
	private int getFirstFat(String name, int tag) {
		int fatIndex = -1; // ͷ��
		for (int i = 0; i < rootManageTable.length; i++) {
			if (rootManageTable[i][11] == tag)
				if (String
					.valueOf(rootManageTable[i], 0, 8)
					.trim()
					.equals(name)) {
					fatIndex = (int) rootManageTable[i][12];

//					System.out.println("Get the first fat item is " + fatIndex);
					break;
				}
		}
		if (fatIndex == -1) {
			System.out.println("Error: Can't get the fat info!");
		}

		return fatIndex;
	}

	// �����Ŀ¼����������Ѱ��ָ��Ŀ¼�ķ���fat����(type ��ʶ 1 Ϊ�ļ���2 ΪĿ¼)
	private int getInnerDirectorySector(List path)
	{
		String tempSt = (String) path.get(0);
		String lookForPath = null;
		int fatIndex = getFirstFat(tempSt, 2);

		// �ҵ���¼��ǰĿ¼����������index
		for (int i = 0; i < path.size() - 1; i++) {
			boolean tag = false;
			for (int j = 0; j < 32; j++) {
				if (datafield[fatIndex][16 * j + 11] == 2) {
					tempSt =
						String.valueOf(datafield[fatIndex], 16 * j, 8).trim();
					lookForPath = (String) path.get(i + 1);
					if (tempSt.trim().equals(lookForPath)) {
						fatIndex = (int) datafield[fatIndex][16 * j + 12];
						tag = true;
						System.out.println(
							"Get the directory fat item is " + fatIndex);
						break;
					}
				}
			}
			if (!tag) {
				System.out.println("can not find the directory " + lookForPath);
				return -1;
			}
		}
		
		return fatIndex;	
	}

	// �������fat��
	private int allocFreeSector() {
		int sectorIndex = -1;

		// �ж� currentBlockIndex �Ƿ�fat��β������Ǿ�ָ��fat��ͷ
		if (currentBlockIndex == fat.length) {
			currentBlockIndex = 0;
		}

		// Ѱ�ұ�ʶΪ���е�fat��Ԫ��
		for (int i = currentBlockIndex; i < fat.length; i++) {
			if (fat[i] == FREE) {
				currentBlockIndex = i;
				fat[i] = END;
				//				eraseContent(i);
				System.out.println("The allocated block is " + i);
				return i;
			}
		}

		// currentBlockIndex�Ժ�û�п��п飬��ͷ��ʼ��
		for (int i = 0; i < fat.length; i++) {
			if (fat[i] == FREE) {
				currentBlockIndex = i;
				fat[i] = END;
				//				eraseContent(i);
				System.out.println("The allocated block is " + i);
				return i;
			}
		}
		return sectorIndex;
	}
	
	public ArrayList getDirectoryContent(List path)
	{
		ArrayList dc = new ArrayList();
		
		// ��ѯ��Ŀ¼
		if (path.size() == 0)
		{
			for (int i = 0; i < rootManageTable.length; i++) {
				// �ļ�
				if (rootManageTable[i][11] == 1)
				{
					String name = String.valueOf(rootManageTable[i], 0, 8).trim();
					
					dc.add("�ļ�     " + name);
				}
				else
				if (rootManageTable[i][11] == 2)
				{
					String name = String.valueOf(rootManageTable[i], 0, 8).trim();
					
					dc.add("Ŀ¼     " + name);
				}
			}
		}
		// ��ѯ�Ǹ�Ŀ¼
		else
		{
			int fatIndex = getInnerDirectorySector(path);

			for (int j = 0; j < 32; j++) {
				if (datafield[fatIndex][16 * j + 11] == 1) {
					String name = String.valueOf(datafield[j], 0, 8).trim();
					
					dc.add("�ļ�     " + name);
				}
				if (datafield[fatIndex][16 * j + 11] == 2) {
					String name = String.valueOf(datafield[j], 0, 8).trim();
					
					dc.add("Ŀ¼     " + name);
				}
			}
		}
		
		return dc; 
	}
	
	void clearContent(int sect)
	{
		for (int i = 0; i < SECTOR; i++)
		{
			datafield[sect][i] = 0;
		}
	}
	
	/*
	
	//	��ȡ��Ŀ¼����Ϣ
	public LinkedList getRootDirectoryInfo()
	{
		LinkedList list = new LinkedList();
		for (int i = 0; i < datafield.length; i++)
		{
			if (datafield[i][11] == 2) //��ΪĿ¼
			{
				char[] bt = new char[12];
				String s = new String(datafield[i], 0, 8).trim();
				String space = new String(bt, 0, 12 - s.length() / 2);
				list.addFirst(s + space + "Ŀ¼");
				//System.out.println("directory: " + space.length());
			}

			if (datafield[i][11] == 1) //��Ϊ�ļ�
			{
				char[] bt = new char[12];
				String name = String.valueOf(datafield[i], 0, 8).trim();
				String extendName = String.valueOf(datafield[i], 8, 3).trim();
				String fullName = "";
				if (extendName.length() == 0)
				{
					fullName = name;
				} else
				{
					fullName = name + "." + extendName;
				}
				String space = new String(bt, 0, 12 - fullName.length() / 2);
				list.addLast(fullName + space + "�ļ�");
			}
		}
		System.out.println(list);
		return list;
	}
	
	LinkedList getDirectoryInfo(List path)
	{
		LinkedList info = new LinkedList();

		if (path.size() == 0)
		{
			info = getRootDirectoryInfo();
			return info;
		}
		int sect = 0;
		if ((sect = this.getFirstFat((String)path.get(0),2)) < 0)
		{
			return info;
		}
		for (int i = 1; i < path.size(); i++)
		{
			if ((sect = this.lookupDirectory(sect, (String) path.get(i))) < 0)
			{
				return info;
			}
		}
		info = this.getDirectoryInfo(sect);
		return info;
	}
	
	//��һָ����������ĳһĿ¼
	int lookupDirectory(int sect, String item)
	{
		for (int i = 0; i < 32; i++)
		{
			if (datafield[sect][i * 16 + 11] == 2)
			{
				if (String
					.valueOf(datafield[sect], i * 16, 8)
					.trim()
					.equals(item))
				{
					return datafield[sect][i * 16 + 12];
				}
			}
		}
		return -1;
	}
	
	LinkedList getDirectoryInfo(int sect)
	{
		LinkedList info = new LinkedList();

		String item = "";
		for (int i = 0; i < 32; i++)
		{
			if (datafield[sect][16 * i + 11] == 2) //��ΪĿ¼
			{
				char[] bt = new char[12];
				String s = new String(datafield[sect], 16 * i, 8).trim();
				String space = new String(bt, 0, 12 - s.length() / 2);
				info.addFirst(s + space + "Ŀ¼");
			}
			if (datafield[sect][16 * i + 11] == 1) //��Ϊ�ļ�
			{
				char[] bt = new char[12];
				String name =
					String.valueOf(datafield[sect], 16 * i, 8).trim();
				String extendName =
					String
						.valueOf(datafield[sect], 16 * i + 8, 3)
						.trim();
				String fullName = "";
				if (extendName.length() == 0)
				{
					fullName = name;
				} else
				{
					fullName = name + "." + extendName;
				}
				String space = new String(bt, 0, 12 - fullName.length() / 2);
				info.addLast(fullName + space + "�ļ�");
			}
		}
		return info;
	}*/
	
}