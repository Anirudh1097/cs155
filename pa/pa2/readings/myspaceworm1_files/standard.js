<!--
// popup function for excerpts
function popUp(pPage) {
while (pPage.substring(0,3) == '../') {
pPage = pPage.substring(3, pPage.length + 1);
}
popUpWin =
window.open('','popWin','resizable=1,scrollbars=1,location=0,toolbar=0,width=525,height=394');
figDoc = popUpWin.document;
zhtm= '<html><head><title>' + pPage + '</title>';
zhtm += '</head>';
zhtm += '<body bgcolor="#FFFFFF">';
zhtm += '<img src="/sfonline/excerpts/images/' + pPage + '">';
zhtm += '<p><b>' + pPage.substring(pPage.lastIndexOf('/') + 1, pPage.length) + '</b> ';
zhtm += '</body></html>';
figDoc.write(zhtm);
figDoc.close();
}
// modified 3.1.99 RWE v4.1 -->

function confirmLink(theLink, theMessage)
{
    // Confirmation is not required in the configuration file
    // or browser is Opera (crappy js implementation)
    if (typeof(window.opera) != 'undefined') {
        return true;
    }

    var is_confirmed = confirm('Are you sure you want to' + ' :\n' + theMessage);

    return is_confirmed;
} // end of the 'confirmLink()' function

function editLinks(nextUrl) {
	//alert(nextUrl);
	//alert(document.forms[0].next.value);
	document.forms[0].next.value = nextUrl;
	//alert(document.forms[0].next.value);
	document.forms[0].submit()
}

function saveForm() {
	var i;
	for(i = 0; i < document.forms[0].goNext.length; i++) {
		if (document.forms[0].goNext[i].checked) {
			document.forms[0].next.value = document.forms[0].goNext[i].value
			break;
		}
	}
	document.forms[0].submit();
}
function openFAQ(url,title) {
	var newWin = window.open('',title,'toolbar=no,scrollbars=yes,menubar=yes,location=no,width=580,height=480,resizable=yes');
	newWin.location.href = url;
	newWin.focus();
}

/******************************************* 
Open and close windows for mapped data types
********************************************/
function openMapped(url,next,name,id) {
	if (!id || id == "") {
		if (confirm("Document must be saved first. Continue?")) {
			if (next !== "") {
				document.forms[0].next.value = next;
			}
			document.forms[0].submit();
			return false;
		} else {
			return false;
		}
	} else {
		if (!url) return; 
		if (!name) name = "SFPSMapped";

		var newWin = window.open('',name,'scrollbars=no,menubar=no,location=no,width=550,height=400,resizable=yes');
		newWin.location.href = url;
		newWin.focus();
	}
}

function openPrintable(url) {
	var newWin = window.open('','Printable','toolbar=yes,scrollbars=yes,menubar=yes,location=yes,width=700,height=600,resizable=yes');
	newWin.location.href = url;
	newWin.focus();
}

function openEmail(url) {
	var newWin = window.open('','Email','toolbar=yes,scrollbars=yes,menubar=yes,location=yes,width=600,height=600,resizable=yes');
	newWin.location.href = url;
	newWin.focus();
}

function openMappedEdit(url,field,next,name,id) {
	if (!id || id == "") {
		if (confirm("Document must be saved first. Continue?")) {
			if (next !== "") {
				document.forms[0].next.value = next;
			}
			document.forms[0].submit();
			return false;
		} else {
			return false;
		}
	} else {
		if (!url) return; 
		if (!name) name = "SFPSMapped";

		var image_field = field;
		url = url + image_field.options[image_field.selectedIndex].value;

		var newWin = window.open('',name,'scrollbars=no,menubar=no,location=no,width=550,height=400,resizable=yes');
		newWin.location.href = url;
		newWin.focus();
	}
}

function deleteMapped(url,field,next,name,id) {
	if (!id || id == "") {
		if (confirm("Document must be saved first. Continue?")) {
			if (next !== "") {
				document.forms[0].next.value = next;
			}
			document.forms[0].submit();
			return false;
		} else {
			return false;
		}
	} else {
		if (!url) return; 

		var mapped_field = field;
		url = url + mapped_field.options[mapped_field.selectedIndex].value;
		document.location.href = url;
	}
}

function closeMapped(win) {
	if (win) { win.close(); }
}
function refreshOpener() {
	document.forms[0].submit();
	if (opener) { 
		var form = opener.document.forms[0];
		form.op.value = "edit";
		form.replicate.value = "false";
		form.submit();
	}
}

/********************************************
 *	JOBS									*
 ********************************************/
// Preview resume
function previewResume(f)
{
	f.op.value='preview';
	f.target='_preview';
	window.open('','_preview','copyhistory=false,height=600,width=600,menubar=false,resizable,scrollbars,status=false,toolbar=false');
	f.submit();
	return true;
}
// Post resume
function postResume(f)
{
	f.op.value='save';
	f.target='';
	return true;
}

function updateStateProv(f)
{
	f.act.value = 'refresh';
	f.submit();
	return true;
}

function toggleMostPopular ( f ) {
	var checked;
	var i;
	var field;
	var fieldnum = 1;
	if (f.most_popular.checked == true)
		checked = true;
	else
		checked = false;

	for(i = 1; i <= f.popular_count.value; i++) {
		eval("f.popular_" + i + ".checked = checked");
	}
}

function toggleNew ( f ) {
    var checked;
    var i;
    var field;
	var fieldnum = 1;
    if (f.all_new_lists.checked == true)
        checked = true;
    else
        checked = false;

    for(i = 1; i <= f.new_count.value; i++) {
        eval("f.new_" + i + ".checked = checked");
    }
}

function toggleOther ( f ) {
    var checked;
    var i;
    var field;
    var fieldnum = 1;
    if (f.all_other_lists.checked == true)
        checked = true;
    else
        checked = false;

    for(i = 1; i <= f.other_count.value; i++) {
        eval("f.other_" + i + ".checked = checked");
    }
}
