
#[derive(Debug)]
struct MySuperSliceable<T: ?Sized> {
    info: u32,
    data: T,
}

fn main() {
    let sized: MySuperSliceable<[u8; 8]> = MySuperSliceable {
            info: 7,
            data: [0; 8],
    };
    
    let dynamic: &MySuperSliceable<[u8]> = &sized;
    println!("{:?} {} {:?}", dynamic, dynamic.info, &dynamic.data);
}
