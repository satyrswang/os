import javax.swing.*;
import java.awt.*;

/**
 * Created by dell on 2015/5/20.
 */
public class Help extends JFrame {
    private Icon specification=new ImageIcon("image/specification.png");
    private JButton button=new JButton();

    //构造函数
    public Help(){
        setLayout(null);
        setSize(520, 520);
        button.setIcon(specification);
        button.setBackground(Color.WHITE);
        button.setSize(520, 510);
        button.setLocation(0,0);
        add(button);
        setVisible(true);
    }
}
