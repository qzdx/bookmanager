import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
class Book {
    public String name;
    public String publ;
    public String isbn;
    public String auth;
    public int num;
    public int price;

    public Book() {}

    public Book(String name, String publ, String isbn, String auth, int num, int price) {
        this.name = name;
        this.publ = publ;
        this.isbn = isbn;
        this.auth = auth;
        this.num = num;
        this.price = price;
    }

    public void show() {
        System.out.println("[" + name + "] {" + isbn + "} [还剩 " + num + "] [价格 " + price + " $]");
    }

    public String shows() {
        return "《 " + name + " 》 价格 " + price + " 剩余数量 " + num + "\n";
    }

    public void input(Scanner scanner) {
        scanner.nextLine(); // 清除缓冲区
        System.out.print("请输入书名: ");
        name = scanner.nextLine();
        System.out.print("请输入出版社: ");
        publ = scanner.nextLine();
        System.out.print("请输入ISBN: ");
        isbn = scanner.nextLine();
        System.out.print("请输入作者: ");
        auth = scanner.nextLine();
        
        System.out.print("请输入库存数量: ");
        while (true) {
            try {
                num = Integer.parseInt(scanner.nextLine());
                if (num >= 0) break;
                System.out.print("输入错误，请输入非负整数: ");
            } catch (NumberFormatException e) {
                System.out.print("输入错误，请输入整数: ");
            }
        }

        System.out.print("请输入价格: ");
        while (true) {
            try {
                price = Integer.parseInt(scanner.nextLine());
                if (price >= 0) break;
                System.out.print("输入错误，请输入非负整数: ");
            } catch (NumberFormatException e) {
                System.out.print("输入错误，请输入整数: ");
            }
        }
    }

    public void serialize(PrintWriter writer) {
        writer.println(name);
        writer.println(publ);
        writer.println(isbn);
        writer.println(auth);
        writer.println(num);
        writer.println(price);
    }

    public static Book deserialize(Scanner fileScanner) {
        if (!fileScanner.hasNextLine()) return null;
        return new Book(
            fileScanner.nextLine(),
            fileScanner.nextLine(),
            fileScanner.nextLine(),
            fileScanner.nextLine(),
            Integer.parseInt(fileScanner.nextLine()),
            Integer.parseInt(fileScanner.nextLine())
        );
    }
}

class BookOS {
    public int cnt;
    public List<Book> books;

    public BookOS() {
        cnt = 0;
        books = new ArrayList<>();
    }

    public int lookup(String isbn) {
        for (int i = 0; i < books.size(); i++) {
            if (books.get(i).isbn.equals(isbn)) {
                return i;
            }
        }
        return -1;
    }

    public int add(Book b) {
        if (lookup(b.isbn) != -1) {
            return -1;
        }
        cnt++;
        books.add(b);
        return 1;
    }

    public int del(String isbn) {
        int index = lookup(isbn);
        if (index == -1) return -1;
        cnt--;
        books.remove(index);
        return 1;
    }

    public void show() {
        for (Book b : books) {
            b.show();
        }
    }

    public void showPrice() {
        if (books.isEmpty()) {
            System.out.println("没有书籍");
            return;
        }
        
        double avg = books.stream()
                         .mapToInt(b -> b.price)
                         .average()
                         .getAsDouble();
        System.out.printf("平均价格: %.2f\n", avg);
        
        List<Book> sorted = new ArrayList<>(books);
        sorted.sort(Comparator.comparingInt(b -> b.price));
        sorted.forEach(Book::show);
    }
}

class BOS {
    public static void hint() {
        System.out.println("1. 添加书籍");
        System.out.println("2. 删除书籍");
        System.out.println("3. 显示所有书籍");
        System.out.println("4. 按价格统计");
        System.out.println("5. 保存数据");
        System.out.println("6. 加载数据");
        System.out.println("0. 退出系统");
    }

    public static void oswarn(String msg) {
        System.out.println("\u001B[31m[!] " + msg + "\u001B[0m");
    }

    public static void osgood(String msg) {
        System.out.println("\u001B[32m[✓] " + msg + "\u001B[0m");
    }
}






// Book、BookOS、BOS、FileHandler类保持不变，与原始代码相同

public class MainGUI extends Frame {
    private BookOS bookOS = new BookOS();
    private TextArea infoArea = new TextArea(20, 60);
    private Label statusLabel = new Label("就绪");

    public MainGUI() {
        super("图书管理系统");
        setupUI();
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                dispose();
                System.exit(0);
            }
        });
    }

    private void setupUI() {
        // 按钮面板
        Panel buttonPanel = new Panel(new FlowLayout());
        String[] buttons = {"添加", "删除", "显示", "统计", "保存", "加载", "退出"};
        for (String btn : buttons) {
            Button button = new Button(btn);
            button.addActionListener(new ActionHandler());
            buttonPanel.add(button);
        }

        // 信息展示区域
        infoArea.setEditable(false);
        
        // 状态栏
        Panel statusPanel = new Panel(new FlowLayout(FlowLayout.LEFT));
        statusPanel.add(statusLabel);

        // 主界面布局
        setLayout(new BorderLayout());
        add(buttonPanel, BorderLayout.NORTH);
        add(infoArea, BorderLayout.CENTER);
        add(statusPanel, BorderLayout.SOUTH);
        
        setSize(800, 600);
        setVisible(true);
    }

    private class ActionHandler implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            String cmd = ((Button)e.getSource()).getLabel();
            switch(cmd) {
                case "添加": showAddDialog(); break;
                case "删除": showDeleteDialog(); break;
                case "显示": showBooks(); break;
                case "统计": showStatistics(); break;
                case "保存": saveData(); break;
                case "加载": loadData(); break;
                case "退出": System.exit(0);
            }
        }
    }

    private void showAddDialog() {
        Dialog dlg = new Dialog(this, "添加书籍", true);
        dlg.setLayout(new GridLayout(7, 2));
        
        TextField[] fields = new TextField[6];
        String[] labels = {"书名:", "出版社:", "ISBN:", "作者:", "数量:", "价格:"};
        
        for(int i=0; i<labels.length; i++) {
            dlg.add(new Label(labels[i]));
            fields[i] = new TextField();
            dlg.add(fields[i]);
        }

        Button okBtn = new Button("确定");
        okBtn.addActionListener(e -> {
            try {
                Book b = new Book(
                    fields[0].getText(),
                    fields[1].getText(),
                    fields[2].getText(),
                    fields[3].getText(),
                    Integer.parseInt(fields[4].getText()),
                    Integer.parseInt(fields[5].getText())
                );
                
                if(bookOS.add(b) > 0) {
                    statusLabel.setText("添加成功");
                } else {
                    statusLabel.setText("ISBN已存在");
                }
                dlg.dispose();
            } catch (NumberFormatException ex) {
                statusLabel.setText("请输入有效的数字");
            }
        });

        dlg.add(new Label());
        dlg.add(okBtn);
        dlg.pack();
        dlg.setVisible(true);
    }

    private void showDeleteDialog() {
        Dialog dlg = new Dialog(this, "删除书籍", true);
        dlg.setLayout(new FlowLayout());
        
        TextField isbnField = new TextField(20);
        Button okBtn = new Button("确定");
        
        okBtn.addActionListener(e -> {
            int result = bookOS.del(isbnField.getText());
            statusLabel.setText(result > 0 ? "删除成功" : "ISBN不存在");
            dlg.dispose();
        });

        dlg.add(new Label("输入ISBN:"));
        dlg.add(isbnField);
        dlg.add(okBtn);
        dlg.pack();
        dlg.setVisible(true);
    }

    private void showBooks() {
        if(bookOS.books.isEmpty()) {
            infoArea.setText("-- 无库存书籍 --");
            return;
        }
        
        StringBuilder sb = new StringBuilder();
        for(Book b : bookOS.books) {
            sb.append(b.shows());
        }
        infoArea.setText(sb.toString());
    }

    private void showStatistics() {
        if(bookOS.books.isEmpty()) {
            infoArea.setText("没有书籍");
            return;
        }
        
        double avg = bookOS.books.stream()
                       .mapToInt(b -> b.price)
                       .average()
                       .getAsDouble();
        
        List<Book> sorted = new ArrayList<>(bookOS.books);
        sorted.sort(Comparator.comparingInt(b -> b.price));
        
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("平均价格: %.2f\n\n", avg));
        for(Book b : sorted) {
            sb.append(b.shows());
        }
        infoArea.setText(sb.toString());
    }

    private void saveData() {
        try {
            FileHandler.saveBooks(bookOS.books, "books.dat");
            statusLabel.setText("数据保存成功");
        } catch (Exception e) {
            statusLabel.setText("保存失败: " + e.getMessage());
        }
    }

    private void loadData() {
        try {
            bookOS.books = FileHandler.loadBooks("books.dat");
            statusLabel.setText("数据加载成功");
            showBooks();
        } catch (Exception e) {
            statusLabel.setText("加载失败: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        new MainGUI();
    }
}

// 修改FileHandler支持异常抛出
class FileHandler {
    public static void saveBooks(List<Book> books, String filename) throws IOException {
        try (PrintWriter writer = new PrintWriter(new FileWriter(filename))) {
            books.forEach(b -> b.serialize(writer));
        }
    }

    public static List<Book> loadBooks(String filename) throws FileNotFoundException {
        List<Book> result = new ArrayList<>();
        try (Scanner scanner = new Scanner(new File(filename))) {
            while (scanner.hasNextLine()) {
                Book b = Book.deserialize(scanner);
                if (b != null) result.add(b);
            }
        }
        return result;
    }
}