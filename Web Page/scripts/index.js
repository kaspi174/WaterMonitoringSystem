function exportToText() {
    /* Get table data */
    var table = document.getElementById("myTable");
    var rows = Array.from(table.rows).map(function(row) {
      return Array.from(row.cells).map(function(cell) {
        return cell.innerText;
      }).join('\t');
    }).join('\n');

    /* Create text file and download */
    var filename = "myTable.txt";
    var blob = new Blob([rows], { type: "text/plain;charset=utf-8" });
    if (navigator.msSaveBlob) { // For IE 10+
      navigator.msSaveBlob(blob, filename);
    } else {
      var link = document.createElement("a");
      if (link.download !== undefined) { // Feature detection
        // Browsers that support HTML5 download attribute
        var url = URL.createObjectURL(blob);
        link.setAttribute("href", url);
        link.setAttribute("download", filename);
        link.style.visibility = 'hidden';
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
      }
    }
  }
