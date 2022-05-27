import { Component, OnInit } from '@angular/core';
import { faLink } from '@fortawesome/free-solid-svg-icons';
import { AuthService } from '@auth0/auth0-angular';

@Component({
  selector: 'app-home-content',
  templateUrl: './home-content.component.html',
  styleUrls: ['./home-content.component.css']
})
export class HomeContentComponent implements OnInit {
  faLink = faLink;

  isLogged = false;

  profileJson: string = null;

  constructor(
    public auth: AuthService,
  ) { }

  ngOnInit() {
    this.auth.user$.subscribe(
      (profile) => {
        if (profile) {
          this.isLogged = true;
          this.profileJson = JSON.stringify(profile, null, 2);
          this.auth.getAccessTokenSilently().subscribe((tkn) => {
            localStorage.setItem('token', tkn);
          });
        }
      }
    );
  }

}
