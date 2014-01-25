function viewSourceCode(file)
{
/*
  var nav = navigator;
  var platform = nav.platform;
  var app = nav.appName;
  alert('platform = '+platform);
*/

  var w = 2*top.screen.width/3;
  var h = 2*top.screen.height/3; // 300;
  
  if (top.counter)
    {
      top.win.close;
      top.win = null;
    }
  else
    {
      top.counter = 0;
    }
  
  top.view = 'view'+top.counter;
  top.win = open(file, top.view, 'scrollbars=yes, left='+ (top.screen.width-w)+ ', top='+(top.screen.height-h)/2+', width='+w+', height='+h);
  top.counter++;
}
