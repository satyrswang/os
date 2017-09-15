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
	 * 管理结构说明：
	 *   一个扇区大小为512 char
	 *   level 1：磁盘总信息     大小：1个扇区大小
	 *   level 2：根目录管理表   大小：1个扇区大小（32*16 其中32为根目录一共可支持文件、目录总数，16为每个管理信息的长度）
	 *   level 3：fat表          大小：4个扇区大小，管理2048个索引（分别对应数据区的2048个数据扇区）
	 *   level 4：数据区         大小：2048个扇区，每个扇区大小为512 char
	 */

	final static int SECTOR = 512; // 一个扇区大小为512 char 
	char[] diskInfo = new char[SECTOR]; // 磁盘总信息 大小为一个扇区
	char[][] rootManageTable = new char[32][16]; // 根目录管理表 大小为一个扇区
	char[] fat = new char[SECTOR * 4]; // fat表大小为2048 char，分别对应数据区的2048个扇区
	char[][] datafield = new char[2048][SECTOR]; // 数据区 2048 * 512 

	final static int END = 0xffff; //用在fat表里, 表示该扇区是数据文件的最后一个扇区.
	final static int FREE = 0xfffe; //fat项数值，标识此fat项对应的数据扇区为空闲
	int currentBlockIndex = 0;

	// 格式化:把fat表清空 + 根目录管理表所有项标记为空闲
	public void format() {
		for (int i = 0; i < fat.length; i++) {
			fat[i] = FREE;
		}
		for (int i = 0; i < rootManageTable.length; i++) {
			rootManageTable[i][11] = 0;
		}
		currentBlockIndex = 0;
	}

	// 在根目录创建项目
	public boolean createRootItem(String name, int itemType) {
		name = name.trim();

		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the directory name has a limitation of 8 byte.");
			return false;
		}

		// 查看根目录是否有同名的项目名
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

		// 查找根目录管理表的空闲信息块，记录项目信息
		for (int i = 0; i < rootManageTable.length; i++) {
			if (rootManageTable[i][11] == 0) {
				// 写入项目的名字
				for (int j = 0; j < name.length(); j++) {
					rootManageTable[i][j] = name.charAt(j);
				}
				for (int k = name.length(); k < 8; k++) {
					rootManageTable[i][k] = 0;
				}

				// 修改类型标记，目录为：2； 文件为：1
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
	
	// 保存文件
	public boolean saveFile(List path,String name,String content)
	{
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the file name has a limitation of 8 byte.");
			return false;
		}
		
		int fatFirstIndex = -1;
		// 保存的文件在根目
		if (path.size()==0)
		{
			// 在fat表中寻找为此文件分配的fat头项
			fatFirstIndex = getFirstFat(name, 1);
			if (fatFirstIndex == -1) {
				return false;
			}
		}
		else
		// 保存的文件在非根目录
		if (path.size()>0)
		{
			fatFirstIndex = getInnerDirectorySector(path);
			if (fatFirstIndex == -1)
			{
				return false;
			}
			
			// 目录扇区表中寻找为此文件分配的fat头项
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
		
		// 为file分配足够的空间
		int fatIndex = fatFirstIndex;
		int blockNum = content.length() / 512; // 应该分配的数据扇区数目
		if (content.length() % 512 != 0) // 加上不足一个扇区的部分
			{
			blockNum++;
		}

		for (int i = 1; i < blockNum; i++) // 分配扇区
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

		// 把内容写入文件
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
	
	// 读取文件
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
		// 读取的文件在根目
		if (path.size()==0)
		{
			// 在fat表中寻找为此文件分配的fat头项
			fatFirstIndex = getFirstFat(name, 1);
			if (fatFirstIndex == -1) {
				return null;
			}
		}
		else
		// 读取的文件在非根目录
		if (path.size()>0)
		{
			fatFirstIndex = getInnerDirectorySector(path);
			if (fatFirstIndex == -1)
			{
				return null;
			}
			
			// 目录扇区表中寻找为此文件分配的fat头项
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
		
		//把内容读入到容器
			 do {
				 stringContainor.append(
					 String.valueOf(datafield[fatFirstIndex], 0, SECTOR).trim());

				fatFirstIndex = fat[fatFirstIndex];
			 } while (fatFirstIndex != END);

//			 System.out.println(fatFirstIndex);
		
		return stringContainor;
	}

	//	 删除根目录上的项目
	public boolean deleteRootItem(String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}

		// 得到项目的头fat项
		int fatIndex = getFirstFat(name, itemType);
		if (fatIndex == -1) {
			return false;
		}

		// 修改fat标记，标识为空闲
		int temp;
		do {
			temp = fat[fatIndex];
			fat[fatIndex] = FREE;
			fatIndex = temp;
		} while (fatIndex != END);

		// 修改根目录管理表项，标记为空闲
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


	// 在非根目录创建项目
	public boolean createItem(List path, String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}

		// 在根目录创建
		if (path.size() == 0) {

		}
		// 在非根目录创建
		else if (path.size() > 0) {
			
			// 找到记录当前目录索引的扇区index
			int fatIndex = getInnerDirectorySector(path);
			if (fatIndex == -1)
			{
				return false;
			}

			// 检查当前目录是否有相同的项目名
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

			// 在当前目录扇区记录目录信息
			for (int i = 0; i < 32; i++) {
				if (datafield[fatIndex][16 * i + 11] == 0) {
					// 写入目录名字
					for (int j = 0; j < name.length(); j++) {
						datafield[fatIndex][16 * i + j] = name.charAt(j);
					}
					for (int k = name.length(); k < 8; k++) {
						datafield[fatIndex][16 * i + k] = 0;
					}

					// 修改类型标记，目录为：2； 文件为：1
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

	//	 删除非根目录上的项目
	public boolean deleteItem(List path, String name, int itemType) {
		name = name.trim();
		if (name.length() > 8) {
			System.out.println(
				"Error:The length of the item name has a limitation of 8 byte.");
			return false;
		}
		
		// 找到记录当前目录索引的扇区index
		int fatIndex = getInnerDirectorySector(path);
		if (fatIndex == -1)
		{
			return false;
		}

		// 检查当前目录是否有此项目,获得头fat坐标
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
					
					itemIndex = i;  // 记下当前的目录记录号
					fatTemp = fatIndex;  // 记下当前目录记录的扇区
					
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

		// 修改fat标记，标识为空闲
		int temp;
		do {
			temp = fat[fatIndex];
			fat[fatIndex] = FREE;
			fatIndex = temp;
		} while (fatIndex != END);

		// 修改根目录管理表项，标记为空闲
		datafield[fatTemp][16*itemIndex +11] = 0;
		datafield[fatTemp][16*itemIndex +12] = 0;

		System.out.println(
		"item " + name + " has been successfully deleted!");

		return true;
	}
	

	// 在根目录管理表表中寻找为文件或目录分配的fat头项 (tag 标识 1 为文件，2 为目录)
	private int getFirstFat(String name, int tag) {
		int fatIndex = -1; // 头项
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

	// 在深层目录管理扇区中寻找指定目录的分配fat坐标(type 标识 1 为文件，2 为目录)
	private int getInnerDirectorySector(List path)
	{
		String tempSt = (String) path.get(0);
		String lookForPath = null;
		int fatIndex = getFirstFat(tempSt, 2);

		// 找到记录当前目录索引的扇区index
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

	// 分配空闲fat项
	private int allocFreeSector() {
		int sectorIndex = -1;

		// 判断 currentBlockIndex 是否到fat表尾，如果是就指会fat表头
		if (currentBlockIndex == fat.length) {
			currentBlockIndex = 0;
		}

		// 寻找标识为空闲的fat表元素
		for (int i = currentBlockIndex; i < fat.length; i++) {
			if (fat[i] == FREE) {
				currentBlockIndex = i;
				fat[i] = END;
				//				eraseContent(i);
				System.out.println("The allocated block is " + i);
				return i;
			}
		}

		// currentBlockIndex以后并没有空闲块，从头开始找
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
		
		// 查询根目录
		if (path.size() == 0)
		{
			for (int i = 0; i < rootManageTable.length; i++) {
				// 文件
				if (rootManageTable[i][11] == 1)
				{
					String name = String.valueOf(rootManageTable[i], 0, 8).trim();
					
					dc.add("文件     " + name);
				}
				else
				if (rootManageTable[i][11] == 2)
				{
					String name = String.valueOf(rootManageTable[i], 0, 8).trim();
					
					dc.add("目录     " + name);
				}
			}
		}
		// 查询非根目录
		else
		{
			int fatIndex = getInnerDirectorySector(path);

			for (int j = 0; j < 32; j++) {
				if (datafield[fatIndex][16 * j + 11] == 1) {
					String name = String.valueOf(datafield[j], 0, 8).trim();
					
					dc.add("文件     " + name);
				}
				if (datafield[fatIndex][16 * j + 11] == 2) {
					String name = String.valueOf(datafield[j], 0, 8).trim();
					
					dc.add("目录     " + name);
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
	
	//	提取根目录的信息
	public LinkedList getRootDirectoryInfo()
	{
		LinkedList list = new LinkedList();
		for (int i = 0; i < datafield.length; i++)
		{
			if (datafield[i][11] == 2) //若为目录
			{
				char[] bt = new char[12];
				String s = new String(datafield[i], 0, 8).trim();
				String space = new String(bt, 0, 12 - s.length() / 2);
				list.addFirst(s + space + "目录");
				//System.out.println("directory: " + space.length());
			}

			if (datafield[i][11] == 1) //若为文件
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
				list.addLast(fullName + space + "文件");
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
	
	//在一指定扇区查找某一目录
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
			if (datafield[sect][16 * i + 11] == 2) //若为目录
			{
				char[] bt = new char[12];
				String s = new String(datafield[sect], 16 * i, 8).trim();
				String space = new String(bt, 0, 12 - s.length() / 2);
				info.addFirst(s + space + "目录");
			}
			if (datafield[sect][16 * i + 11] == 1) //若为文件
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
				info.addLast(fullName + space + "文件");
			}
		}
		return info;
	}*/
	
}