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
                "Assign   : Token name, Expr value",
                "Binary   : Expr left, Token operator, Expr right",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Logical  : Expr left, Token operator, Expr right",
                "Unary    : Token operator, Expr right",
                "Variable : Token name"));
        defineAst(output_dir, "Stmt", Arrays.asList(
                "Block      : List<Stmt> statements",
                "Expression : Expr expression",
                "If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
                "Print      : Expr expression",
                "Var        : Token name, Expr initializer",
                "While      : Expr condition, Stmt body"));
    }

    private static void defineAst(String output_dir, String base_name, List<String> types) throws IOException {
        String path = output_dir + "/" + base_name + ".java";
        PrintWriter writer = new PrintWriter(path, "UTF-8");

        writer.println("package lox;\n");
        writer.println("import java.util.List;\n");
        writer.println("abstract class " + base_name + " {");
        defineVisitor(writer, base_name, types);
        for (String type : types) {
            String class_name = type.split(":")[0].trim();
            String fields = type.split(":")[1].trim();
            defineType(writer, base_name, class_name, fields);
        }
        writer.println("\n\tabstract <R> R accept(Visitor<R> visitor);");
        writer.print("}");
        writer.close();
    }

    private static void defineVisitor(PrintWriter writer, String base_name, List<String> types) {
        writer.println("\tinterface Visitor<R> {");
        for (String type : types) {
            String type_name = type.split(":")[0].trim();
            writer.println(
                    "\t\tR visit" + type_name + base_name + "(" + type_name + " " + base_name.toLowerCase() + ");\n");
        }
        writer.println("\t}");
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

        writer.println("\t\t@Override");
        writer.println("\t\t<R> R accept(Visitor<R> visitor) {");
        writer.println("\t\t\treturn visitor.visit" + class_name + base_name + "(this);");
        writer.println("\t\t}\n");

        for (String field : fields) {
            writer.println("\t\tfinal " + field + ";");
        }

        writer.println("\t}");
    }
}
