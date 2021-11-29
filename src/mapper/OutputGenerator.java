package mapper;

import java.io.FileReader;
import java.io.BufferedReader;
import java.lang.Exception;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class OutputGenerator{
    public static void main(String[] args){
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0]))) {
            List<String> param_names = new ArrayList<String>();
            List<String> value_names = new ArrayList<String>();
            while (reader.ready()) {
                String[] words = reader.readLine().split("\t");
                param_names.add(words[0]);
                for(int i = 1; i < words.length; i++){
                    value_names.add(words[i]);
                }
            }

            Scanner scanner = new Scanner(System.in);
            int counter = 0;

            System.out.print("样例ID");
            for(int i = 0; i < param_names.size(); i++){
                System.out.print("\t" + param_names.get(i));
            }
            System.out.println("\t新增的元组数");

            while (scanner.hasNext()) {
                counter++;
                System.out.print(String.valueOf(counter));
                String[] words = scanner.nextLine().split(" ");
                for(int i = 0; i < words.length - 1; i++){
                    System.out.print("\t" + value_names.get(Integer.parseInt(words[i])));
                }
                System.out.println("\t" + words[words.length - 1]);
            }
        }
        catch (Exception e) {} 
    }
}

// Reference: https://stackoverflow.com/questions/3663944/what-is-the-best-way-to-remove-the-first-element-from-an-array/3663949
// Reference: https://beginnersbook.com/2013/12/how-to-copy-and-add-all-list-elements-to-arraylist-in-java/