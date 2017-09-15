import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class Warning extends JFrame{
    private JLabel label=new JLabel("您的输入不正确或内存空间不足！",JLabel.CENTER);
    private JButton button=new JButton("确定");

    //构造函数
    public Warning(){
        setLayout(null);
        setSize(500, 200);
        setLocation(800, 200);
        label.setSize(500, 50);
        label.setFont(new java.awt.Font("Dialog", 1, 23));
        label.setForeground(Color.red);
        add(label);
        button.setSize(160, 50);
        button.setLocation(170, 80);
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });
        add(button);
        setVisible(true);
    }
}
