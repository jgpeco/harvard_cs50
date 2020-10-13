//dropdown on buttons in about
const detailsButton = document.querySelectorAll('.item-button')



detailsButton.forEach(button => {
    button.onclick = () => {
        const grid = button.parentNode.parentNode
        const gridItems = grid.childNodes
        gridItems.forEach(item => {
            if(!item.classList) return;
            if(item.classList.value === 'item-info_full'){
                if(item.style.display == 'block'){
                    item.style.display = 'none'
                } else {
                    item.style.display = 'block'
                }
            }
        })
    }
})