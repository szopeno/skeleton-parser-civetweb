function uploadJobundle(oData) {

    alert('Hello poczatek!');
    var form = document.getElementById('file-form');
    var fileSelect = document.getElementById('file-select');
    var uploadButton = document.getElementById('upload-button');
 // event.preventDefault();

  var files = fileSelect.files;
  var formData = new FormData();
  
 for (var i = 0; i < files.length; i++) {
  var file = files[i];

  /*
  if (!file.type.match('image.*')) {
    continue;
  }
   */

  //formData.append('jobundle_sizes[]', file.size);
  //formData.append('jobundle_names[]', file.name);
  formData.append('jobundles[]', file, file.name);
 }

  var xhr = new XMLHttpRequest();
 xhr.overrideMimeType("text/plain; charset=x-user-defined");

  xhr.open('POST', '/jobs/templates', true);
  
    xhr.onload = function () {
        alert('ding?');
      if (xhr.status === 200) {
	// File(s) uploaded.
	uploadButton.innerHTML = 'Upload';
        alert('udalo sie');
      } else {
	alert('An error occurred!');
      }
    };
        alert('sending!');

  xhr.send(formData);
}
