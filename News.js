export default function News(news){
    return `
    <section id="news">
    <h1 class="title">News</h1>
        <div class="search">
            <input type="text" name='news' placeholder="Search News...">
        </div>
    <div class="news-list">
        ${NewsItems(news)}
    </div>
    </section>
    `
}

export function NewsItems(news){
    console.log("news", news);
    return news.slice(0,4).map(d=>`
        <div class="row">
            <div class="col-3">
                ${d.title}
            </div>
            <div class="col-2">
                ${d.date}
            </div>
        </div>
    `).join('');
}


export function handleNewsFilter(data){
    let input = document.querySelector('.search input[name="news"]')
    console.log(input);
    input.addEventListener('input', function(event){
    if (this.value!=''){
        let filtered = data.news.filter(d=>{
            let text = (d.title + ' ' + d.date);
            return text.toLowerCase().includes(this.value.toLowerCase());
        })
        console.log('chec', filtered);
        document.querySelector('.news-list').innerHTML = NewsItems(filtered);
    }else{
        document.querySelector('.news-list').innerHTML = NewsItems(data.news);
        } 
    });
}