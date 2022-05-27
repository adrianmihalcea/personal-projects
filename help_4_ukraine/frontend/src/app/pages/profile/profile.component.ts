import { Component, OnInit } from '@angular/core';
import { AuthService } from '@auth0/auth0-angular';

@Component({
  selector: 'app-profile',
  templateUrl: './profile.component.html',
  styleUrls: ['./profile.component.css'],
})
export class ProfileComponent implements OnInit {
  profileJson: string = null;

  constructor(public auth: AuthService) {}

  ngOnInit() {
    this.auth.user$.subscribe(
      (profile) => {
        this.profileJson = JSON.stringify(profile, null, 2);
        this.auth.getAccessTokenSilently().subscribe((tkn) => {
          console.log("---------");
          console.log(tkn);
        });
        // const accessToken = await this.auth.getAccessTokenSilently().toPromise().then(
        //   (tkn) => {
        //     console.log(accessToken);
        //   }
        // )
        
      }
    );
  }
}
