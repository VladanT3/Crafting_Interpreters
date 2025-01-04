package tool;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.List;

public class GenerateAst {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: generate_ast <output directory>");
            System.exit(64);
        }
        String output_dir = args[0];
        defineAst(output_dir, "Expr", Arrays.asList(
                "Binary   : Expr left, Token operator, Expr right",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Unary    : Token operator, Expr right"));
    }

    private static void defineAst(String output_dir, String base_name, List<String> types) throws IOException {
        String path = output_dir + "/" + base_name + ".java";
        PrintWriter writer = new PrintWriter(path, "UTF-8");

        writer.println("package lox;\n");
        writer.println("import java.util.List;\n");
        writer.print("abstract class " + base_name + " {");
        for (String type : types) {
            String class_name = type.split(":")[0].trim();
            String fields = type.split(":")[1].trim();
            defineType(writer, base_name, class_name, fields);
        }
        writer.print("}");
        writer.close();
    }

    private static void defineType(PrintWriter writer, String base_name, String class_name, String field_list) {
        writer.println("\n\tstatic class " + class_name + " extends " + base_name + " {");
        writer.println("\t\t" + class_name + "(" + field_list + ") {");

        String[] fields = field_list.split(", ");
        for (String field : fields) {
            String name = field.split(" ")[1];
            writer.println("\t\t\tthis." + name + " = " + name + ";");
        }

        writer.println("\t\t}\n");

        for (String field : fields) {
            writer.println("\t\tfinal " + field + ";");
        }

        writer.println("\t}");
    }
}
