
  // Import the functions you need from the SDKs you need
  //import { initializeApp } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-app.js";
  //import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-analytics.js";
  // TODO: Add SDKs for Firebase products that you want to use
  // https://firebase.google.com/docs/web/setup#available-libraries

  // Your web app's Firebase configuration
  // For Firebase JS SDK v7.20.0 and later, measurementId is optional
  const firebaseConfig = {
    apiKey: "AIzaSyAI-C7yosSDwE96kym5dZRxgVQY1Hra6Rw",
    authDomain: "project3-2803f.firebaseapp.com",
    databaseURL: "https://project3-2803f-default-rtdb.firebaseio.com",
    projectId: "project3-2803f",
    storageBucket: "project3-2803f.appspot.com",
    messagingSenderId: "332981991249",
    appId: "1:332981991249:web:c7b5d29806f20261764570",
    //measurementId: "G-4JP5YDYD8V"
  };

  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);

  var database = firebase.database();
  //const analytics = getAnalytics(app);

var btnOn = document.getElementById("btnOnId_01");
var btnOff = document.getElementById("btnOffId_01");

function onClick(){
    var w = document.getElementById("kl").value;
    //console.log(w);
    database.ref().update({
        "weight": Number(w)
    })
}

btnOn.onclick = function(){
    database.ref().update({
        "pump_web": 1
    })
}

btnOff.onclick = function(){
    database.ref().update({
        "pump_web": 0
    })
}

database.ref("/led").on("value", function(snapshot){
    var led = snapshot.val();
    if(led >= 1){
        document.getElementById("den").src = "./led_on.jpg";
    }else{
        document.getElementById("den").src = "./led_off.png";
    }
})

//Lấy giá trị cảnh báo trộm
database.ref("/warn").on("value", function(snapshot){
    var warn = snapshot.val();
    if(warn <= 0){
        document.getElementById("canhbao").innerHTML = "Không phát hiện đột nhập. An toàn!";
    }else{
        document.getElementById("canhbao").innerHTML = "Phát hiện đột nhập";
    }
})

//Lấy giá trị nhiệt độ, độ ẩm
database.ref("/tem").on("value", function(snapshot){
    var tem = snapshot.val();
    document.getElementById("nhietdo").innerHTML = tem;
})

database.ref("/hum").on("value", function(snapshot){
    var hum = snapshot.val();
    document.getElementById("doam").innerHTML = hum;
})

//Lấy giá trị máy bơm
database.ref("/pump").on("value", function(snapshot){
    var pump = snapshot.val();
    if(pump >= 1){
        document.getElementById("maybom").innerHTML = "Máy bơm đang bật";
    }else{
        document.getElementById("maybom").innerHTML = "Máy bơm đang tắt";
    }
})

