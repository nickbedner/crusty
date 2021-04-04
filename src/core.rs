#[no_mangle]
pub extern "C" fn rust_double(n: i32) -> i32 {
  n * n
}
