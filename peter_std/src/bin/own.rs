fn main() {
    let s = String::from("hello, world!");
    let mut iter1 = s.chars();
    let mut iter2 = (s.chars()).skip(3);
    println!("{:?}", iter1.next());
    println!("{:?}", iter2.next());
}
