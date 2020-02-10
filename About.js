export default function About(about){
    return `
    <section id="about">
        <h1 class="title animated infinite bounce delay-1s">${about.name}</h1>
        <div class="row">
            <div class="col-1">
                <img class="profile-img" src="${about.photo}"/>
                <p>
                    <strong>${about.title}</strong><br>
                    ${about.email} <br>
                    ${about.address}<br>
                    <a href="${about.resume}" target="_blank"><i class="far fa-file-alt"></i> Resume</a> |
                    <a href="${about.github}" target="_blank"><i class="fab fa-github"></i> Github</a><br>
                </p>
            </div>
            <div class="col-1">
                <p>
                ${about.desc}
                </p>
            </div
        </div >    
    </section>`
}