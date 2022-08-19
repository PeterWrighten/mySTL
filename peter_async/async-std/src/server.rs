use async_std::{
    prelude::*,
    task,
    net::{TcpListener, ToSocketAddrs, TcpStream},
    io::BufReader,
};

use futures::channel::mpsc;
use futures::sink::SinkExt;
use std::{sync::Arc, hash::Hash};
use std::collections::hash_map::{Entry, HashMap};

type Result<T> = std::result::Result<T, Box<dyn std::error::Error + Send + Sync>>;
type Sender<T> = mpsc::UnboundedSender<T>;
type Receiver<T> = mpsc::UnboundedReceiver<T>;


#[derive(Debug)]
enum Event {
    NewPeer {
        name: String,
        stream: Arc<TcpStream>,
    },
    Message {
        from: String,
        to: Vec<String>,
        msg: String,
    },
}

async fn broker_loop(mut events: Receiver<Event>) -> Result<()> {
    let mut peers: HashMap<String, Sender<String>> = HashMap::new();

    while let Some(event) = events.next().await {
        match event {
            Event::Message{ from, to, msg } => {
                for addr in to {
                    if let Some(peer) = peers.get_mut(&addr) {
                        let msg = format!("from {}: {}\n", from, msg);
                        peer.send(msg).await?;
                    }
                }
            }

            Event::NewPeer { name, stream } => {
                match peers.entry(name) {
                    Entry::Occupied(..) => (),
                    Entry::Vacant(entry) => {
                        let (client_sender, client_receiver) = mpsc::unbounded();
                        entry.insert(client_sender);
                        spawn_and_log_error(connection_writer_loop(client_receiver, stream));
                    }
                }
            }
        }
    }
    Ok(())
}

async fn connection_writer_loop(
    mut messages: Receiver<String>,
    stream: Arc<TcpStream>,
) -> Result<()> {
    let mut stream = &*stream;
    while let Some(msg) = messages.next().await {
        stream.write_all(msg.as_bytes()).await?;
    }
    Ok(())
}


async fn accept_loop(addr: impl ToSocketAddrs) -> Result<()> {
    let listener = TcpListener::bind(addr).await?;
    let mut incoming = listener.incoming();
    while let Some(stream) = incoming.next().await {
        let stream = stream?;
        println!("Accepting from: {}", stream.peer_addr()?);
        let _handle = task::spawn(connect_loop(stream));
    }
    Ok(())
}

async fn run() -> Result<()> {
    let fut = accept_loop("localhost:5659");
    task::block_on(fut)
}

async fn connect_loop(stream: TcpStream) -> Result<()> {
    let reader = BufReader::new(&stream);
    let mut lines = reader.lines();

    let name = match lines.next().await {
        Some(line) => line?,
        None => Err("peer disconnected immediately")?,
    };
    println!("name = {}", name);

    while let Some(line) = lines.next().await {
        let line = line?;
        let (dst, msg) = match line.find(':') {
            Some(idx) => {
                (&line[..idx], line[idx+1..].trim())
            },
            None => continue,
        };
        let dst: Vec<String> = dst.split(',').map(|name| name.trim().to_string()).collect();
        let msg = msg.to_string();
    }
    Ok(())
}

// Handle errors: log them and continue serving them
fn spawn_and_log_error<F>(fut: F) -> task::JoinHandle<()> 
    where
        F: Future<Output = Result<()>> + Send + 'static,
    {
        task::spawn(async move {
            if let Err(e) = fut.await {
                eprintln!("{}", e)
            }
        })
    }