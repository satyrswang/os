import javax.swing.*;
import java.awt.*;

/**
 * Created by dell on 2015/5/13.
 */

class MyFrame extends JFrame {
    private MyPanel panel = new MyPanel();
    //构造函数
    public MyFrame() {
        setSize(1260, 800);
        Container contentPane = getContentPane();
        contentPane.add(panel);
        setTitle("Memory Allocation");
        setResizable(false);
        setVisible(true);
    }
    //Run（）函数，使得repaint（）一直被调用，从而达到实时更新的效果
    public void Run() {
        while (true) {
            repaint();
        }

    }

}

