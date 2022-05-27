trait Animal {
    fn snuggle(&self);
    fn eat(&mut self);
}

trait Cat: Animal {
    fn meow(&self);
}

trait Dog: Animal {
    fn bark(&self);
}


