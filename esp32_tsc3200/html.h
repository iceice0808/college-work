const char html_page[] PROGMEM = R"RawString(
<!DOCTYPE html>
<html>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #f4f4f4;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
    }

    h1 {
      text-align: center;
      font-size: 36px;
      color: #333;
    }

    p {
      text-align: center;
      font-size: 18px;
      color: #555;
    }

    #combined-box {
      width: 100px;
      height: 100px;
      margin-top: 20px;
    }

    #save-button {
      margin-top: 20px;
      padding: 10px;
      font-size: 16px;
      background-color: #3498db;
      color: #fff;
      border: none;
      cursor: pointer;
    }

    #blend-button {
      margin-top: 10px;
      padding: 10px;
      font-size: 16px;
      background-color: #2ecc71;
      color: #fff;
      border: none;
      cursor: pointer;
    }

    #clear-selection-button {
      margin-top: 10px;
      padding: 10px;
      font-size: 16px;
      background-color: #e74c3c;
      color: #fff;
      border: none;
      cursor: pointer;
    }

    #color-list {
      margin-top: 20px;
    }

    table {
      border-collapse: collapse;
      width: 80%;
      margin: auto;
      margin-top: 20px;
      border: 1px solid #ddd;
    }

    th, td {
      border: 1px solid #ddd;
      padding: 8px;
      text-align: center;
    }

    th {
      background-color: #3498db;
      color: white;
    }
  </style>

<body>
  <h1>TCS3200 Color Sensor with ESP32</h1>
  <p>Read and save colors!</p>
  <div id="_R" style="display:none;"></div>
  <div id="_G" style="display:none;"></div>
  <div id="_B" style="display:none;"></div>
  <div id="combined-box"></div>
  <button id="save-button" onclick="saveColor()">Save Color</button>
  <button id="blend-button" onclick="blendSelectedColors()">Blend Selected Colors</button>
  <button id="clear-selection-button" onclick="clearSelection()">Clear Selection</button>
  <div id="color-list"></div>
  <div id="blended-color-list"></div>

<script>
  var savedColors = [];
  var blendedColors = [];
  var selectedColors = [];

  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        const text = this.responseText;
        const myArr = JSON.parse(text);
        document.getElementById("_R").innerHTML = myArr[0];
        document.getElementById("_G").innerHTML = myArr[1];
        document.getElementById("_B").innerHTML = myArr[2];
        
        updateCombinedColorBox(myArr[0], myArr[1], myArr[2]);
      }
    };
    xhttp.open("GET", "readColors", true);
    xhttp.send();
  }, 100);

  function updateCombinedColorBox(red, green, blue) {
    var combinedBox = document.getElementById('combined-box');
    combinedBox.style.backgroundColor = 'rgb(' + red + ',' + green + ',' + blue + ')';
  }

  function saveColor() {
    var currentColor = {
      red: parseFloat(document.getElementById("_R").innerText),
      green: parseFloat(document.getElementById("_G").innerText),
      blue: parseFloat(document.getElementById("_B").innerText)
    };
    savedColors.push(currentColor);
    updateColorList();
    alert("Color saved!");
  }

  function blendSelectedColors() {
    if (selectedColors.length >= 2) {
      var totalRed = 0;
      var totalGreen = 0;
      var totalBlue = 0;

      for (var i = 0; i < selectedColors.length; i++) {
        totalRed += selectedColors[i].red;
        totalGreen += selectedColors[i].green;
        totalBlue += selectedColors[i].blue;
      }

      var blendedColor = {
        red: Math.floor(totalRed / selectedColors.length),
        green: Math.floor(totalGreen / selectedColors.length),
        blue: Math.floor(totalBlue / selectedColors.length)
      };

      blendedColors.push(blendedColor);
      updateBlendedColorList();

      var saveBlended = confirm("Do you want to save the blended color?");
      if (saveBlended) {
        savedColors.push(blendedColor);
        updateColorList();
      }

      alert("Colors blended!");
    } else {
      alert("Please select at least two colors to blend.");
    }
  }

  function updateBlendedColorList() {
    var blendedColorList = document.getElementById('blended-color-list');
    blendedColorList.innerHTML = "<h3>Blended Colors:</h3>";

    if (blendedColors.length === 0) {
      blendedColorList.innerHTML += "<p>No blended colors yet.</p>";
    } else {
      var table = document.createElement('table');
      table.style.borderCollapse = 'collapse';
      table.style.width = '80%';
      table.style.margin = 'auto';
      table.style.marginTop = '20px';

      var headerRow = document.createElement('tr');
      headerRow.innerHTML = '<th>Red</th><th>Green</th><th>Blue</th><th>Color</th>';
      table.appendChild(headerRow);

      for (var i = 0; i < blendedColors.length; i++) {
        var colorRow = document.createElement('tr');
        colorRow.innerHTML = '<td>' + blendedColors[i].red + '</td><td>' + blendedColors[i].green + '</td><td>' + blendedColors[i].blue + '</td><td style="background-color: rgb(' + blendedColors[i].red + ',' + blendedColors[i].green + ',' + blendedColors[i].blue + ');"></td>';
        table.appendChild(colorRow);
      }

      blendedColorList.appendChild(table);
    }
  }

  function updateColorList() {
    var colorList = document.getElementById('color-list');
    colorList.innerHTML = "<h3>Saved Colors:</h3>";

    if (savedColors.length === 0) {
      colorList.innerHTML += "<p>No colors saved yet.</p>";
    } else {
      var form = document.createElement('form');

      for (var i = 0; i < savedColors.length; i++) {
        var colorCheckbox = document.createElement('input');
        colorCheckbox.type = 'checkbox';
        colorCheckbox.name = 'colorOption';
        colorCheckbox.value = i.toString();

        var colorDiv = document.createElement('div');
        colorDiv.style.width = '20px';
        colorDiv.style.height = '20px';
        colorDiv.style.display = 'inline-block';
        colorDiv.style.backgroundColor = 'rgb(' + savedColors[i].red + ',' + savedColors[i].green + ',' + savedColors[i].blue + ')';
        
        var colorLabel = document.createElement('label');
        colorLabel.htmlFor = 'colorOption_' + i;
        colorLabel.innerHTML = 'Red: ' + savedColors[i].red + ', Green: ' + savedColors[i].green + ', Blue: ' + savedColors[i].blue;
        
        form.appendChild(colorCheckbox);
        form.appendChild(colorDiv);
        form.appendChild(colorLabel);
        form.appendChild(document.createElement('br'));
      }

      colorList.appendChild(form);

      form.addEventListener('change', function() {
        selectedColors = [];
        var checkedCheckboxes = document.querySelectorAll('input[name="colorOption"]:checked');
        checkedCheckboxes.forEach(function(checkbox) {
          var selectedIndex = parseInt(checkbox.value);
          selectedColors.push(savedColors[selectedIndex]);
        });
      });
    }
  }


</script>

</body>
</html>
)RawString";