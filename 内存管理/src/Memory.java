/**
 * Created by dell on 2015/5/17.
 */
public class Memory {
    public int start;  //每块内存的起始地址
    public int size;    //每块内存的大小
    public boolean use;    //每块内存的使用情况

    //构造函数
    public Memory(int start,int size,boolean use){
        this.start=start;
        this.size=size;
        this.use=use;
    }

}
