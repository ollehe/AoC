use regex::Regex;
use std::fs::File;
use std::io::{self, BufRead};

fn parse_file(file_path: &str) -> io::Result<Vec<String>> {
    let file = File::open(file_path)?;
    let reader = io::BufReader::new(file);

    // Collecting the lines directly as Vec<String>
    let lines: io::Result<Vec<String>> = reader.lines().collect();
    match lines {
        Ok(vec) => Ok(vec), // Return the vector of strings if successful
        Err(e) => {
            println!("Failed to read file: {}", e);
            Ok(Vec::new()) // Return an empty Vec if there was an error
        }
    }
}

fn remove_white_spaces(lines: &io::Result<Vec<String>>) -> Vec<String> {
    match lines {
        Ok(vec) => {
            // Use `map` to apply `replace` to each line
            vec.iter().map(|line| line.replace(" ", "")).collect()
        }
        Err(e) => {
            println!("Failed to read file: {}", e);
            Vec::new() // Return an empty Vec if there was an error
        }
    }
}

// Take an expression of the form i64 Operator i64 ... where Operator is either + or *
fn eval_left_to_right(expression: &str) -> Result<i64, String> {
    let re = Regex::new(r"(\d+|[+*])").unwrap();
    let split_expression: Vec<_> = re
        .captures_iter(expression)
        .map(|cap| cap[0].to_string())
        .collect();
    let first_num = split_expression[0]
        .parse::<i64>()
        .map_err(|_| format!("Invalid number for operand: {}", split_expression[0]))?;

    split_expression[1..]
        .chunks(2)
        .try_fold(first_num, |acc, next_block| {
            let next_operator = &next_block[0]; // Use & to match string slice
            let next_number = next_block[1]
                .parse::<i64>()
                .map_err(|_| format!("Invalid number for operand: {}", next_block[1]))?;

            match next_operator.as_str() {
                "*" => Ok(acc * next_number),
                "+" => Ok(acc + next_number),
                _ => Err(format!("Unsupported operator: {}", next_operator)),
            }
        })
}
// Take an expression of the form i64 Operator i64 ... where Operator is either + or *
fn eval_plus_first(expression: &str) -> Result<i64, String> {
    let mut plus_eval = expression.to_string();

    let re = Regex::new(r"(\d+[+]\d+)").unwrap();

    //If we have a match
    while let Some(captures) = re.captures(&plus_eval) {
        let first_match = &captures[0];
        match eval_left_to_right(first_match) {
            Ok(val) => {
                plus_eval = plus_eval.replacen(first_match, val.to_string().as_str(), 1);
            }
            Err(e) => {
                println!("Cannot compute {}:{}", first_match, e);
            }
        }
    }

    eval_left_to_right(&plus_eval)
}

// reduce composite expressions of the form (expression Operator (expression))
// and replace it by a simple expression of the form (expression)
// for part 1 use eval_left_to_right and eval_plus_first for part 2
fn reduce_composite_expression(expression: &String) -> String {
    // println!("Reducing composite expression {:?}", expression);
    let mut new_expression = expression.to_string();
    if expression.contains('(') && expression.contains(')') {
        let re = Regex::new(r"\(([^()]*)\)").unwrap(); // matches the substrings expr2 of a string "( expr1 (expr2))"
                                                       // Collect the matches and remove the ( and )
        let simple_expressions: Vec<_> = re
            .find_iter(expression.as_str())
            .map(|m| m.as_str().replace("(", "").replace(")", ""))
            .collect();

        simple_expressions
            .iter()
            .for_each(|simple_exp| match eval_plus_first(simple_exp) {
                // Use eval_left_to_right for part 1 and  eval_plus_first for part 2
                Ok(num) => {
                    new_expression =
                        new_expression.replace(&format!("({})", simple_exp), &num.to_string());
                }
                Err(e) => {
                    println!("Error evaluating expression {}: {}", simple_exp, e);
                }
            });
        return reduce_composite_expression(&new_expression);
    }
    // println!("Expression is reduced {:?}", new_expression);
    new_expression
}
// for part 1 use eval_left_to_right and eval_plus_first for part 2

fn compute_composites(composite_expressions: &Vec<String>) -> Result<i64, String> {
    //Reduce composite expressions
    // println!("Composite expressions: {:?}", composite_expressions);
    let reduced_expressions: Vec<_> = composite_expressions
        .iter()
        .map(reduce_composite_expression)
        .collect();
    // println!("Reduced expressions: {:?}", reduced_expressions);

    //Evaluate simple expressions
    let values: Vec<_> = reduced_expressions
        .iter()
        .map(|simple_exp| eval_plus_first(&simple_exp.as_str())) // Use eval_left_to_right for part 1 and  eval_plus_first for part 2
        .collect();
    // println!("Values using plus first {:?}", values);
    values
        .iter()
        .try_fold(0, |acc, next_val: &Result<i64, String>| match next_val {
            Ok(value) => Ok(acc + value),
            Err(e) => Err(format!("Cannot compute: {}", e)),
        })
}

/* This is the main function. Use a main.rs file to execute it */
pub fn solve() {
    let lines = parse_file("src/input.txt"); // Ensure the file path is correct
    let expressions = remove_white_spaces(&lines);
    println!("The sum is {:?}", compute_composites(&expressions));
}
