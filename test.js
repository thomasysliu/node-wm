var helloeio = require('./build/default/helloworld_eio');
hi = new helloeio.HelloWorldEio();
/*
hi.hello(function(data){
  console.log(data);
});
*/


var io = require('socket.io').listen(8888);

io.sockets.on('connection', function (socket) {
	
	hi.get_screen(function(x,y,color){
  //console.log("x="+x+" y="+y+" color="+color);
  //socket.emit('news', { hello: 'aa' });
  socket.emit('news', { x: x,y:y,c:color });
});

  //socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });
});
