// QQ表情插件
var name="";
var url="ws://192.168.199.180:9999";
var output;
var websocket;
function init() {
	while (name == "null" || name == "" || name.length > 10)
	{
		if (name.length > 10) {
			name = prompt("昵称长度不能超过10，请重新输入");
		} else {
			name = prompt("请输入您的昵称");
		}
	}
	if (name == "null" || name == "" || name.length > 10)
	{
		name = "匿名用户";
	}
	document.getElementById("name").innerHTML="Hello, "+name;
	output = document.getElementById("show");
	document.getElementById("saytext").addEventListener('keydown',function(e){ 		
		if(e.keyCode != 13) {
			return;
		}
		e.preventDefault();
		Send();
	});
	

	initWebSocket();
}

function initWebSocket() {
	if ("WebSocket" in window)
	{
		document.getElementById("name").innerHTML="Hello, "+name+" <span style='color:blue;'>[正在连接]</span>";
		try
		{
			websocket = new WebSocket(url); 
		} catch(e) {  
            alert("服务器异常");
			disconnected();
			return;
        }  
		websocket.onopen = function(evt) { 
			onOpen(evt)
		}; 
		websocket.onclose = function(evt) { 
			onClose(evt) 
		}; 
		websocket.onmessage = function(evt) { 
			onMessage(evt) 
		}; 
		websocket.onerror = function(evt) { 
			onError(evt) 
		}; 
	} else {
		alert("您的浏览器不支持WebScoket");
		disconnected();
	}
}  

function onOpen(evt) { 
	document.getElementById("sub_btn").disabled=false;
	document.getElementById("name").innerHTML="Hello, "+name+" <span style='color:blue;'>[连接成功]</span>";
}  

function onClose(evt) { 
	alert("连接中断"); 
	disconnected();
}  

function onMessage(evt) { 
	writeToScreen(replace_em(evt.data));
}  

function onError(evt) { 
	alert("服务器异常");
	disconnected();
}  

function disconnected() {
	document.getElementById("name").innerHTML="Hello, "+name+" <span style='color:blue;'>[连接中断]</span>";
	document.getElementById("sub_btn").disabled=true;
}

function Send() {
	var str = document.getElementById("saytext").value;
	if (str.length > 100)
	{
		alert("发送内容过长");
		return;
	}
	str = "[" + name + "]: " + str
	websocket.send(str);
	document.getElementById('saytext').value = "";
	document.getElementById('saytext').focus();
	writeToScreen(replace_em(str));
}

function writeToScreen(message) { 
        var pre = document.createElement("p"); 
        pre.style.wordWrap = "break-word"; 
        pre.innerHTML = message; 
        output.appendChild(pre); 
		output.scrollTop = output.scrollHeight;
} 


//查看结果
function replace_em(str){
	str = str.replace(/\</g,'&lt;');
	str = str.replace(/\>/g,'&gt;');
	str = str.replace(/\n/g,'<br/>');
	str = str.replace(/\[em_([0-9]*)\]/g,'<img src="face/$1.gif" border="0" />');
	return str;
}


$(function(){
	$('.emotion').qqFace({
		id : 'facebox', //表情盒子的ID
		assign:'saytext', //给那个控件赋值
		path:'face/'	//表情存放的路径
	});
});

// QQ表情插件
(function($){  
	$.fn.qqFace = function(options){
		var defaults = {
			id : 'facebox',
			path : 'face/',
			assign : 'content',
			tip : 'em_'
		};
		var option = $.extend(defaults, options);
		var assign = $('#'+option.assign);
		var id = option.id;
		var path = option.path;
		var tip = option.tip;
		
		if(assign.length<=0){
			alert('缺少表情赋值对象。');
			return false;
		}
	
		$(this).click(function(e){
			var strFace, labFace;
			if($('#'+id).length<=0){
				strFace = '<div id="'+id+'" style="position:absolute;display:none;z-index:1000;" class="qqFace">' +
							  '<table border="0" cellspacing="0" cellpadding="0"><tr>';
				for(var i=1; i<=75; i++){
					labFace = '['+tip+i+']';
					strFace += '<td><img src="'+path+i+'.gif" onclick="$(\'#'+option.assign+'\').setCaret();$(\'#'+option.assign+'\').insertAtCaret(\'' + labFace + '\');" /></td>';
					if( i % 15 == 0 ) strFace += '</tr><tr>';
				}
				strFace += '</tr></table></div>';
			}
			$(this).parent().append(strFace);
			var offset = $(this).position();
			var top = offset.top + $(this).outerHeight();
			$('#'+id).css('top',top);
			$('#'+id).css('left',offset.left);
			$('#'+id).show();
			e.stopPropagation();
		});

		$(document).click(function(){
			$('#'+id).hide();
			$('#'+id).remove();
			
		});
	};

})(jQuery);

jQuery.extend({ 
unselectContents: function(){ 
	if(window.getSelection) 
		window.getSelection().removeAllRanges(); 
	else if(document.selection) 
		document.selection.empty(); 
	} 
}); 
jQuery.fn.extend({ 
	selectContents: function(){ 
		$(this).each(function(i){ 
			var node = this; 
			var selection, range, doc, win; 
			if ((doc = node.ownerDocument) && (win = doc.defaultView) && typeof win.getSelection != 'undefined' && typeof doc.createRange != 'undefined' && (selection = window.getSelection()) && typeof selection.removeAllRanges != 'undefined'){ 
				range = doc.createRange(); 
				range.selectNode(node); 
				if(i == 0){ 
					selection.removeAllRanges(); 
				} 
				selection.addRange(range); 
			} else if (document.body && typeof document.body.createTextRange != 'undefined' && (range = document.body.createTextRange())){ 
				range.moveToElementText(node); 
				range.select(); 
			} 
		}); 
	}, 

	setCaret: function(){ 
		if(!$.browser.msie) return; 
		var initSetCaret = function(){ 
			var textObj = $(this).get(0); 
			textObj.caretPos = document.selection.createRange().duplicate(); 
		}; 
		$(this).click(initSetCaret).select(initSetCaret).keyup(initSetCaret); 
	}, 

	insertAtCaret: function(textFeildValue){ 
		var textObj = $(this).get(0); 
		if(document.all && textObj.createTextRange && textObj.caretPos){ 
			var caretPos=textObj.caretPos; 
			caretPos.text = caretPos.text.charAt(caretPos.text.length-1) == '' ? 
			textFeildValue+'' : textFeildValue; 
		} else if(textObj.setSelectionRange){ 
			var rangeStart=textObj.selectionStart; 
			var rangeEnd=textObj.selectionEnd; 
			var tempStr1=textObj.value.substring(0,rangeStart); 
			var tempStr2=textObj.value.substring(rangeEnd); 
			textObj.value=tempStr1+textFeildValue+tempStr2; 
			textObj.focus(); 
			var len=textFeildValue.length; 
			textObj.setSelectionRange(rangeStart+len,rangeStart+len); 
			textObj.blur(); 
		}else{ 
			textObj.value+=textFeildValue; 
		} 
		document.getElementById('saytext').focus();
	} 
});