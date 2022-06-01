use std::arch::asm;

fn main() {
    let i: u64 = 3;
    let o: u64;

    unsafe {
        asm!(
            "mov {0}, {1}",
            "add {0}, 5",
            out(reg) o,
            in(reg) i,
        );
    }

    assert_eq!(o, 8);
    let mut x: usize = 3;
    unsafe {
        asm!(
            "add {0}, 5", inout(reg) x
        );
    }
    println!("{}", x);
    let a: usize = 12;
    let r: usize;
    unsafe {
        asm!(
            "add {0}, 2", inout(reg) a => r
        );
    }
    println!("{}", r);

    let mut c: usize = 4;
    let d: usize = 4;
    let e: usize = 4;
    unsafe {
        asm!(
            "add {0}, {1}",
            "add {0}, {2}",
            inlateout(reg) c,
            in(reg) e,
            in(reg) d,
        );
    }
    println!("{}", c);
}


