import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Rectangle2D;

public class MyPanel extends JPanel {
    private JLabel label=new JLabel("Memory Allocation",JLabel.CENTER);
    private JLabel apply=new JLabel("请输入要申请内存的大小：",JLabel.CENTER);
    private JLabel delete=new JLabel("请选择要释放的内存：",JLabel.CENTER);
    private Icon algorithm[]=new Icon[4];    //放四种算法图片的一个数组
    private JLabel image[]=new JLabel[4];
    private JButton runall=new JButton();    //“全部运行”的按钮
    private JButton runsingle=new JButton();    //“单个运行”的按钮
    private Icon All=new ImageIcon("image/all.png");
    private Icon single=new ImageIcon("image/single.png");
    private Font font=new Font("楷体", Font.PLAIN,18);
    private JTextField input=new JTextField("0-640k",8);
    private JButton sure=new JButton("确定");
    private JButton okay=new JButton("确定");
    private JComboBox deleteBox;
    private JLabel applySet[]=new JLabel[4];
    private JLabel deleteSet[]=new JLabel[4];
    private JTextField inputSet[]=new JTextField[4];
    private JComboBox deleteBoxSet[]=new JComboBox[4];
    private JButton sureSet[]=new JButton[4];
    private JButton okaySet[]=new JButton[4];
    private Controller controller=new Controller();
    private int num=0;
    private int[][]numStart=new int[4][640];
    private JButton reset=new JButton();    //重置按钮
    private JButton help=new JButton();    //帮助按钮
    //构造函数
    MyPanel(){
        setLayout(null);
        label.setSize(300, 50);
        label.setLocation(480, 10);
        label.setFont(new java.awt.Font("Dialog", 1, 23));
        label.setForeground(Color.orange);
        add(label);
        for(int i=0;i<4;i++){
            algorithm[i]=new ImageIcon("image/"+i+".png");
        }
        for(int i=0;i<4;i++){
            image[i]=new JLabel();
            image[i].setIcon(algorithm[i]);
            image[i].setSize(100, 100);
            image[i].setLocation(20, 120 * i + 130);
            add(image[i]);
        }
        for(int i=0;i<4;i++){
            final int j=i;
            applySet[i]=new JLabel("请输入要申请内存的大小：",JLabel.CENTER);
            applySet[i].setSize(350, 50);
            applySet[i].setLocation(740, 130 * i + 120);
            applySet[i].setFont(font);
            applySet[i].setForeground(Color.green);
            add(applySet[i]);
            deleteSet[i]=new JLabel("请选择要释放的内存：",JLabel.CENTER);
            deleteSet[i].setSize(350, 50);
            deleteSet[i].setLocation(723, 130 * i + 160);
            deleteSet[i].setFont(font);
            deleteSet[i].setForeground(Color.green);
            add(deleteSet[i]);
            inputSet[i]=new JTextField("0-640k",8);
            inputSet[i].setLocation(1020, 130 * i + 130);
            inputSet[i].setSize(65, 30);
            inputSet[i].setHorizontalAlignment(JTextField.LEFT);
            add(inputSet[i]);
            sureSet[i]=new JButton("确定");
            sureSet[i].setSize(60, 30);
            sureSet[i].setLocation(1178, 130 * i + 130);
            sureSet[i].setBackground(Color.orange);
            sureSet[i].addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    int start;
                    int size = Integer.parseInt(inputSet[j].getText());
                    controller.applySize = size;
                    switch (j) {   //调用不同的算法来分配请求的内存
                     case 0:
                             start= controller.firstMethod(size);
                             if(start==-1){
                                 new Warning();
                             }
                             else deleteBoxSet[j].addItem("Start: " + start + "   size: " + size);
                             break;
                        case 1:
                             start = controller.secondMethod(size);
                            if(start==-1){
                                new Warning();
                            }
                             else deleteBoxSet[j].addItem("Start: " + start + "   size: " + size);
                             break;
                        case 2:
                             start = controller.thirdMethod(size);
                             if(start==-1){
                                new Warning();
                             }
                             else deleteBoxSet[j].addItem("Start: " + start + "   size: " + size);
                             break;
                        case 3:
                            start = controller.lastMethod(size);
                            if(start==-1){
                                new Warning();
                            }
                            else deleteBoxSet[j].addItem("Start: " + start + "   size: " + size);
                            break;
                        default:
                            break;
                    }
                }
            });
            add(sureSet[i]);
            deleteBoxSet[i]= new JComboBox();
            deleteBoxSet[i].setSize(150, 30);
            deleteBoxSet[i].setLocation(1020, 130 * i + 165);
            add(deleteBoxSet[i]);
            okaySet[i]=new JButton("确定");
            okaySet[i].setSize(60, 30);
            okaySet[i].setLocation(1178, 130 * i + 165);
            okaySet[i].setBackground(Color.orange);
            okaySet[i].addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    String str;
                    int length;
                    int start;
                    int size;
                    switch(j){    //调用不同算法的释放内存的函数
                        case 0:str=(String)deleteBoxSet[j].getSelectedItem();
                               length=str.indexOf("size");
                               start = Integer.parseInt(str.substring(7, length - 3));
                               size = Integer.parseInt(str.substring(length + 6));
                               controller.freeFirst(start,size);
                               deleteBoxSet[j].removeItem(deleteBoxSet[j].getSelectedItem());
                               break;
                        case 1:str=(String)deleteBoxSet[j].getSelectedItem();
                               length=str.indexOf("size");
                               start = Integer.parseInt(str.substring(7, length - 3));
                               size = Integer.parseInt(str.substring(length + 6));
                               controller.freeSecond(start,size);
                               deleteBoxSet[j].removeItem(deleteBoxSet[j].getSelectedItem());
                               break;
                        case 2:str=(String)deleteBoxSet[j].getSelectedItem();
                               length=str.indexOf("size");
                               start = Integer.parseInt(str.substring(7, length - 3));
                               size = Integer.parseInt(str.substring(length + 6));
                               controller.freeThird(start, size);
                               deleteBoxSet[j].removeItem(deleteBoxSet[j].getSelectedItem());
                               break;
                        case 3:str=(String)deleteBoxSet[j].getSelectedItem();
                               length=str.indexOf("size");
                               start = Integer.parseInt(str.substring(7, length - 3));
                               size = Integer.parseInt(str.substring(length + 6));
                               controller.freeLast(start,size);
                               deleteBoxSet[j].removeItem(deleteBoxSet[j].getSelectedItem());
                               break;
                        default:
                            break;
                    }
                }
            });
            add(okaySet[i]);

        }
        runall.setSize(100, 30);
        runall.setLocation(150, 640);
        runall.setIcon(All);
        add(runall);
        apply.setSize(350, 50);
        apply.setLocation(200, 615);
        apply.setFont(font);
        apply.setForeground(Color.green);
        add(apply);
        input.setLocation(490, 625);
        input.setSize(65, 30);
        input.setHorizontalAlignment(JTextField.LEFT);
        add(input);
        sure.setSize(60, 30);
        sure.setLocation(650, 625);
        sure.setBackground(Color.orange);
        sure.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int size = Integer.parseInt(input.getText());
                controller.applySize = size;
                int rsize=controller.firstMethod(size);
                if(rsize==-1)new Warning();
                else {
                    numStart[0][num] = rsize;
                    numStart[1][num] = controller.secondMethod(size);
                    numStart[2][num] = controller.thirdMethod(size);
                    numStart[3][num] = controller.lastMethod(size);
                    deleteBox.addItem("Number: " + num + "   size: " + size);
                    num++;
                }

            }
        });
        add(sure);
        delete.setSize(350, 50);
        delete.setLocation(181, 652);
        delete.setFont(font);
        delete.setForeground(Color.green);
        add(delete);
        deleteBox = new JComboBox();
        deleteBox.setSize(150, 30);
        deleteBox.setLocation(490, 662);
        add(deleteBox);
        okay.setSize(60, 30);
        okay.setLocation(650, 662);
        okay.setBackground(Color.orange);
        okay.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int length = 0;
                String str = (String) deleteBox.getSelectedItem();
                length = str.indexOf("size");
                int start = Integer.parseInt(str.substring(8, length - 3));
                int size = Integer.parseInt(str.substring(length + 6));
                controller.freeFirst(numStart[0][start], size);
                controller.freeSecond(numStart[1][start], size);
                controller.freeThird(numStart[2][start], size);
                controller.freeLast(numStart[3][start], size);
                deleteBox.removeItem(deleteBox.getSelectedItem());
            }
        });
        add(okay);
        runsingle.setSize(100, 30);
        runsingle.setLocation(970, 75);
        runsingle.setIcon(single);
        add(runsingle);
        reset.setIcon(new ImageIcon("image/reset.png"));
        reset.setSize(80, 30);
        reset.setLocation(1150, 635);
        reset.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                controller = new Controller();
                deleteBox.removeAllItems();
                for (int i = 0; i < 4; i++) {
                    deleteBoxSet[i].removeAllItems();
                }
            }
        });
        add(reset);
        help.setIcon(new ImageIcon("image/help.png"));
        help.setSize(80, 30);
        help.setLocation(1150, 682);
        help.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new Help();
            }
        });
        add(help);
        setVisible(true);
    }

   //在panel上画出矩形代表内存
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        Rectangle2D memory[]=new Rectangle2D[4];
        for(int i=0;i<4;i++) {
            g2.setPaint(Color.black);
            g2.drawString("0",150,130*i+118);
            g2.drawString("640",790,130*i+118);
            g2.setPaint(Color.pink);
            memory[i]=new Rectangle2D.Double(150, 130 * i + 120,640,100);
            g2.draw(memory[i]);
            g2.fill(memory[i]);
        }
        controller.paint(g2);
        paintBorder(g2);
    }


    //画边框的函数
    private void paintBorder(Graphics2D g2) {
        g2.setPaint(Color.ORANGE);
        for(int i=0;i<controller.fMemories.size();i++){
            int start = controller.fMemories.get(i).start;
            int size =controller.fMemories.get(i).size;
            if(controller.fMemories.get(i).use){
                g2.draw(new Rectangle2D.Double(150 + start, 120, size, 100));
            }
        }
        for(int i=0;i<controller.sMemories.size();i++){
            int start =controller.sMemories.get(i).start;
            int size =controller.sMemories.get(i).size;
            if(controller.sMemories.get(i).use){
                g2.draw(new Rectangle2D.Double(150 + start, 250, size, 100));
            }
        }
        for(int i=0;i<controller.tMemories.size();i++){
            int start = controller.tMemories.get(i).start;
            int size =controller.tMemories.get(i).size;
            if(controller.tMemories.get(i).use){
                g2.draw( new Rectangle2D.Double(150 + start, 380, size, 100));
            }
        }
        for(int i=0;i<controller.lMemories.size();i++){
            int start = controller.lMemories.get(i).start;
            int size =controller.lMemories.get(i).size;
            if(controller.lMemories.get(i).use){
                g2.draw(new Rectangle2D.Double(150 + start, 510, size, 100));
            }
        }
    }

}
