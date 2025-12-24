/* scripts.js
*/
document.querySelector('.copy-btn').addEventListener('click', function() {
    // Get the element you want to copy the content from
    var contentToCopy = document.getElementById('content-to-copy');

    // Create a range object to select the content
    var range = document.createRange();
    range.selectNode(contentToCopy);
    
    // Select the text
    window.getSelection().removeAllRanges(); // Clear current selection
    window.getSelection().addRange(range); // Select the content

    // Copy the text to the clipboard
    try {
        var successful = document.execCommand('copy');
        var msg = successful ? 'Copied!' : 'Failed to copy';
        console.log(msg);
        alert(msg); // Optional: show a confirmation alert
    } catch (err) {
        console.error('Copy command was unsuccessful: ', err);
    }
    
    // Clear selection after copy
    window.getSelection().removeAllRanges();
});
