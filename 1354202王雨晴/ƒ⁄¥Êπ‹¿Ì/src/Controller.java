import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;

public class Controller {
    public int applySize;        //要申请的内存的大小
    public int ThirdTag=0;       //为第三个算而设置的标记
    public ArrayList<Memory>fMemories=new ArrayList<Memory>();      //这四个是来存放四个内存的使用情况的数组
    public ArrayList<Memory>sMemories=new ArrayList<Memory>();
    public ArrayList<Memory>tMemories=new ArrayList<Memory>();
    public ArrayList<Memory>lMemories=new ArrayList<Memory>();

    //构造函数，初始化四个数组
    public Controller() {
        fMemories.add(new Memory(0,640,false));
        sMemories.add(new Memory(0,640,false));
        tMemories.add(new Memory(0,640,false));
        lMemories.add(new Memory(0,640,false));
    }

    //根据每块内存不同的使用情况为不同的内存填充不同的颜色来区分
    public void paint(Graphics2D g2) {
        Color color = g2.getColor();
        for(int i=0;i<fMemories.size();i++){
            int start = fMemories.get(i).start;
            int size =fMemories.get(i).size;
            if(fMemories.get(i).use){
                String Sstart=String.valueOf(start);
                String Send=String.valueOf(size+start);
                g2.setPaint(Color.black);
                g2.drawString(Sstart,150 + start,118);
                g2.drawString(Send,150 + start+size,118);
                g2.setPaint(Color.cyan);
                Rectangle2D rec = new Rectangle2D.Double(150 + start, 120, size, 100);
                g2.fill(rec);
            }
        }
        for(int i=0;i<sMemories.size();i++){
            int start = sMemories.get(i).start;
            int size =sMemories.get(i).size;
            if(sMemories.get(i).use){
                String Sstart=String.valueOf(start);
                String Send=String.valueOf(size+start);
                g2.setPaint(Color.black);
                g2.drawString(Sstart,150 + start,248);
                g2.drawString(Send, 150 + start + size, 248);
                g2.setPaint(Color.cyan);
                Rectangle2D rec = new Rectangle2D.Double(150 + start, 250, size, 100);
                g2.fill(rec);
            }
        }
        for(int i=0;i<tMemories.size();i++){
            int start = tMemories.get(i).start;
            int size =tMemories.get(i).size;
            if(tMemories.get(i).use){
                String Sstart=String.valueOf(start);
                String Send=String.valueOf(size+start);
                g2.setPaint(Color.black);
                g2.drawString(Sstart,150 + start,378);
                g2.drawString(Send,150 + start+size,378);
                g2.setPaint(Color.cyan);
                g2.setPaint(Color.cyan);
                Rectangle2D rec = new Rectangle2D.Double(150 + start, 380, size, 100);
                g2.fill(rec);
            }
        }
        for(int i=0;i<lMemories.size();i++){
            int start = lMemories.get(i).start;
            int size =lMemories.get(i).size;
            if(lMemories.get(i).use){
                String Sstart=String.valueOf(start);
                String Send=String.valueOf(size+start);
                g2.setPaint(Color.black);
                g2.drawString(Sstart,150 + start,508);
                g2.drawString(Send,150 + start+size,508);
                g2.setPaint(Color.cyan);
                g2.setPaint(Color.cyan);
                Rectangle2D rec = new Rectangle2D.Double(150 + start, 510, size, 100);
                g2.fill(rec);
            }
        }
        g2.setPaint(color);
    }

    //最先适配算法的实现
    public int firstMethod(int applySize) {
        int startPositoin=-1;
        if(applySize<0)
            return startPositoin;
        for (int i = 0; i < fMemories.size(); i++) {
            if (!fMemories.get(i).use) {
                if (fMemories.get(i).size > applySize) {
                    startPositoin = fMemories.get(i).start;
                    int wholeSize = fMemories.get(i).size;
                    fMemories.remove(i);
                    fMemories.add(i, new Memory(startPositoin, applySize, true));
                    fMemories.add(i + 1, new Memory(startPositoin + applySize, wholeSize - applySize, false));
                    break;
                } else if (fMemories.get(i).size == applySize) {
                    fMemories.get(i).use = true;
                    startPositoin = fMemories.get(i).start;
                    break;
                }
            }
        }
        return startPositoin;
    }

    //最先适配算法的释放内存
    public void freeFirst(int start,int size){
        for (int i = 0; i < fMemories.size(); i++) {
            if (fMemories.get(i).use) {
                if (fMemories.get(i).start ==start) {
                    if(!fMemories.get(i+1).use){
                        int nextSize=fMemories.get(i+1).size;
                        fMemories.remove(i);
                        fMemories.add(i,new Memory(start,size+nextSize,false));
                    }
                    else
                        fMemories.get(i).use=false;
                }
            }
        }
    }

    //最佳适配算法的实现
    public int secondMethod(int applySize){
        int startPositoin=-1;
        int minSize=640;
        int index=0;
        boolean tag=false;
        if(applySize<0)
            return startPositoin;
       for (int i = 0; i < sMemories.size(); i++) {
            if (!sMemories.get(i).use) {
                if (!(sMemories.get(i).size < applySize)) {
                    minSize = sMemories.get(i).size;
                    startPositoin = sMemories.get(i).start;
                    index=i;
                    tag=true;
                    break;
                }
            }
        }
        if(!tag){
            return startPositoin;
        }
        for (int i = 0; i < sMemories.size(); i++) {
            if (!sMemories.get(i).use) {
                if (sMemories.get(i).size > applySize) {
                    if(sMemories.get(i).size < minSize){
                        startPositoin = sMemories.get(i).start;
                        minSize= sMemories.get(i).size;
                        index=i;
                    }
                }
            }
        }
        if(sMemories.get(index).size>applySize){
            int wholeSize = sMemories.get(index).size;
            sMemories.remove(index);
            sMemories.add(index, new Memory(startPositoin, applySize, true));
            sMemories.add(index+ 1, new Memory(startPositoin + applySize, wholeSize - applySize, false));
        }
        else if(sMemories.get(index).size==applySize) {
            sMemories.get(index).use = true;
        }
        return startPositoin;
    }

    //最佳适配算法的释放内存
    public void freeSecond(int start,int size){
        for (int i = 0; i < sMemories.size(); i++) {
            if (sMemories.get(i).use) {
                if (sMemories.get(i).start ==start) {
                    if(!sMemories.get(i+1).use){
                        int nextSize=sMemories.get(i+1).size;
                        sMemories.remove(i);
                        if(i<640)
                            sMemories.remove(i);
                        sMemories.add(i,new Memory(start,size+nextSize,false));
                    }
                    else
                        sMemories.get(i).use=false;
                }
            }
        }
    }

    //循环最先适配算法的实现
    public int thirdMethod(int applySize) {
        int startPositoin = -1;
        if(applySize<0)
            return startPositoin;
        for (int i = 0; i < tMemories.size(); i++) {
            if (ThirdTag+applySize>640) {
                ThirdTag = 0;
            }
            if (tMemories.get(i).start > ThirdTag||tMemories.get(i).start == ThirdTag) {
                if (!tMemories.get(i).use) {
                    if (tMemories.get(i).size > applySize) {
                        startPositoin = tMemories.get(i).start;
                        int wholeSize = tMemories.get(i).size;
                        tMemories.remove(i);
                        tMemories.add(i, new Memory(startPositoin, applySize, true));
                        tMemories.add(i + 1, new Memory(startPositoin + applySize, wholeSize - applySize, false));
                        break;
                    } else if (tMemories.get(i).size == applySize) {
                        tMemories.get(i).use = true;
                        startPositoin = tMemories.get(i).start;
                        break;
                    }
                }
            }
        }
        ThirdTag=startPositoin+applySize;
        return startPositoin;
    }

    //循环最先适配算法的内存释放
    public void freeThird(int start,int size){
        for (int i = 0; i < tMemories.size(); i++) {
            if (tMemories.get(i).use) {
                if (tMemories.get(i).start ==start) {
                    if(!tMemories.get(i+1).use){
                        int nextSize=tMemories.get(i+1).size;
                        tMemories.remove(i);
                        tMemories.add(i,new Memory(start,size+nextSize,false));
                    }
                    else
                        tMemories.get(i).use=false;
                }
            }
        }
    }

    //最坏适配算法的实现
    public int lastMethod(int applySize){
        int startPositoin=-1;
        int maxSize=0;
        int index=0;
        boolean tag=false;
        if(applySize<0)
            return startPositoin;
        for (int i = 0; i < lMemories.size(); i++) {
            if (!lMemories.get(i).use) {
                if (!(lMemories.get(i).size < applySize)) {
                    maxSize = lMemories.get(i).size;
                    startPositoin = lMemories.get(i).start;
                    index=i;
                    tag=true;
                    break;
                }
            }
        }
        if(!tag){
            return startPositoin;
        }
        for (int i = 0; i < lMemories.size(); i++) {
            if (!lMemories.get(i).use) {
                if (lMemories.get(i).size > applySize) {
                    if(lMemories.get(i).size > maxSize){
                        startPositoin = lMemories.get(i).start;
                        maxSize= lMemories.get(i).size;
                        index=i;
                    }
                }
            }
        }
        if(lMemories.get(index).size>applySize){
            int wholeSize =lMemories.get(index).size;
            lMemories.remove(index);
            lMemories.add(index, new Memory(startPositoin, applySize, true));
            lMemories.add(index+1, new Memory(startPositoin + applySize, wholeSize - applySize, false));
        }
        else if(lMemories.get(index).size==applySize) {
            lMemories.get(index).use = true;
        }
        return startPositoin;
    }

    //最坏适配算法的释放内存
    public void freeLast(int start,int size){
        for (int i = 0; i < lMemories.size(); i++) {
            if (lMemories.get(i).use) {
                if (lMemories.get(i).start ==start) {
                    if(!lMemories.get(i+1).use){
                        int nextSize=lMemories.get(i+1).size;
                        lMemories.remove(i);
                        lMemories.add(i,new Memory(start,size+nextSize,false));
                    }
                    else
                        lMemories.get(i).use=false;
                }
            }
        }
    }
}



