package mapper;

import java.io.FileReader;
import java.io.BufferedReader;
import java.lang.Exception;
import java.util.ArrayList;
import java.util.List;

public class InputGenerator{
    public static void main(String[] args){
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0]))) {
            List<Integer> list = new ArrayList<Integer>();
            while (reader.ready()) {
                String line = reader.readLine();
                list.add(line.split("\t").length - 1);
            }

            System.out.println(list.size());
            for(int count : list){
                System.out.println(count);
            }
        }
        catch (Exception e) {} 
    }
}

// Reference: https://stackoverflow.com/questions/34954630/java-read-line-using-inputstream