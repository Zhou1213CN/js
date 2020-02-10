export default function Navbar(page, items){
    return `
    <nav>
        <ul>
           ${page==='project'? (
                `<li>
                    <a href=".">Go Back</a>
                </li>`
           ):(
                items.map(d=>
                `<li>
                    <a href="#${d}">${d.toUpperCase()}</a>
                </li>
                `).join('')
            )}
        </ul>

    </nav>`
}

