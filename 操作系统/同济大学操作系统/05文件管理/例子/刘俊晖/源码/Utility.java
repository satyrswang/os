/*
 * Created on 2004-5-29
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class Utility {
	//ĳ��Ŀ¼���ļ�������Ϣ(�����ı��ļ�����Ŀ¼)�Ĵ�С��char��
	static public char SIZE_OF_FILEINFO = 16;

	//ģ�������Ĵ�С
	static public char SIZE_OF_SECTOR = 512;

	//����Ŀ¼������Ŀ¼���ļ���Ϣ�������Ŀ��Ĭ������һ��������š���
	static public char NUM_OF_ROOTFILE =
		(char) (SIZE_OF_SECTOR / SIZE_OF_FILEINFO);

	//������������Ŀ¼���ļ���Ϣ�������Ŀ��Ĭ��Ҳֻ��һ��������
	static public char NUM_OF_SUBFILE =
		(char) (SIZE_OF_SECTOR / SIZE_OF_FILEINFO);

	//�������ܵ���������(Ĭ����5M�����ݿռ�)
	static public char NUM_OF_DATASECTOR = 3 * 1024;

	//Fat���б�ʾ�����ı��
	static public char END_OF_FILE = '#'; //0xffff; 	
	static public char FREE_FOR_FAT = '*'; //0xfffe;

	//�ļ���Ϣ�������Ϣ��λ
	//���У�12λ���ļ�����1λ״̬λ��1λFat����ʼλ��1λ�ļ���С(Ŀ¼��λ����)��1λ���ֻ��
	static public char POS_NAME = 0;
	static public char LEN_OF_NAME = 12;
	static public char POS_STATE = 12;
	static public char POS_FAT = 13;
	static public char POS_SIZE = 14;
	static public char POS_READONLY = 15;

	//�ļ���Ϣ��״̬��־����
	static public char FREE = 100;
	static public char FILE = 101;
	static public char DIRECTORY = 102;

	//�ļ���ռ�����������
	static public char MAX_SECTOR = 100;

	//	�������
	static public final int WIDTH = 600;
	static public final int HEIGHT = 600;
	
	//Ĭ�ϵ������ļ���
	static public final String INIT_FILE ="deploy.ini";
}
